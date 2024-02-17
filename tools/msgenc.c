#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "util.h"

#define MAP_STRING_SZ_MAX 16
struct msgenc_charmap_entry {
    char string[MAP_STRING_SZ_MAX];
    size_t string_sz;
    uint8_t value;
};
struct msgenc_charmap {
    struct msgenc_charmap_entry* map;
    size_t map_len;
};

struct msgenc_charmap msgenc_charmap_read(const char* filename) {
    char* data;
    size_t data_sz;
    data = util_read_whole_file(filename, &data_sz);

    // simplifies the code
    if (data[data_sz - 1] != '\n') {
        util_fatal_error("msgenc_charmap_read: %s must end with a line return", filename);
    }

    size_t n_lines = 0;
    {
        for (size_t i = 0; i < data_sz; i++) {
            if (data[i] == '\n') {
                n_lines++;
            }
        }
    }

    struct msgenc_charmap charmap;
    charmap.map = malloc(sizeof(struct msgenc_charmap_entry[n_lines]));
    if (charmap.map == NULL) {
        perror("malloc");
        util_fatal_error("msgenc_charmap_read: malloc failed");
    }
    charmap.map_len = 0;

    for (size_t i = 0; i < data_sz;) {
        // Note: The file is UTF-8 which is self-synchronizing
        // https://en.wikipedia.org/wiki/Self-synchronizing_code
        // This means searching for the bytes corresponding to a character
        // is the same as searching for that character.

        char* line = data + i;

        // find end of line
        char* eol = memchr(line, '\n', data_sz - i);
        assert(eol != NULL); // because the file ends in \n

        if (line == eol) {
            // skip empty lines
            i++;
            continue;
        }

        struct msgenc_charmap_entry* entry = &charmap.map[charmap.map_len];
        charmap.map_len++;

        // NUL-terminate the line, to use string function
        *eol = '\0';

        char* first_space = strchr(line, ' ');
        char* last_space = strrchr(line, ' ');
        if (first_space == NULL || last_space == NULL)
            util_fatal_error("msgenc_charmap_read: missing space separator in line \"%s\"", line);

        // string

        entry->string_sz = first_space - line;
        if (entry->string_sz == 0)
            util_fatal_error("msgenc_charmap_read: no string before space separator in line \"%s\"", line);
        if (entry->string_sz >= MAP_STRING_SZ_MAX)
            util_fatal_error("msgenc_charmap_read: string is too long, in line \"%s\"", line);
        memcpy(entry->string, line, entry->string_sz);
        entry->string[entry->string_sz] = '\0';

        // value

        char* value_str = last_space + 1;
        if (value_str == eol)
            util_fatal_error("msgenc_charmap_read: no value after space separator in line \"%s\"", line);

        char* endptr;
        long value = strtol(value_str, &endptr, 16);
        if (endptr != eol || value < 0 || value > 0xFF)
            util_fatal_error("msgenc_charmap_read: invalid value \"%s\" in line \"%s\"", value_str, line);

        entry->value = (uint8_t)value;

        // next line

        i = eol + 1 - data;
    }

    free(data);

    return charmap;
}

void msgenc_charmap_free(struct msgenc_charmap* charmap) {
    free(charmap->map);
    charmap->map = NULL;
    charmap->map_len = 0;
}

void msgenc_process(FILE* f_in, FILE* f_out, struct msgenc_charmap charmap) {

    const size_t buffer_sz = 32 * 1024;
    char* buffer = malloc(buffer_sz);
    if (buffer == NULL) {
        perror("malloc");
        util_fatal_error("msgenc_process: malloc buffer failed");
    }
    size_t avail = 0; // how many bytes are available in the buffer for processing
    bool is_eof = false;
    bool is_line_start = true; // are we at the first character in the current line
    bool is_hash_line = false; // current line starts with "#"
    int n_backslashes = 0;     // how many consecutive backslashes '\' precede the current character
    bool is_in_string = false; // are we inside a string literal (outside of hash lines)
    while (!is_eof) {
        size_t nread = fread(buffer + avail, 1, buffer_sz - avail, f_in);
        avail += nread;
        if (avail < buffer_sz) {
            int fread_errno = errno;
            if (feof(f_in)) {
                is_eof = true;
            } else if (nread == 0) {
                errno = fread_errno;
                perror("fread");
                util_fatal_error("msgenc_process: fread error");
            }
        }
        size_t processed_end = 0;
        assert(is_eof || avail > MAP_STRING_SZ_MAX);
        size_t process_end = avail - (is_eof ? 0 : MAP_STRING_SZ_MAX);
        for (size_t i = 0; i < process_end;) {
            if (buffer[i] == '#') {
                if (is_line_start) {
                    is_hash_line = true;
                }
            }
            if (buffer[i] == '"') {
                if (!is_in_string) {
                    if (!is_hash_line) {
                        is_in_string = true;
                    }
                } else {
                    if (n_backslashes % 2 == 0) {
                        is_in_string = false;
                    }
                }
            }
            if (buffer[i] == '\\') {
                n_backslashes++;
            } else {
                n_backslashes = 0;
            }
            if (buffer[i] == '\n') {
                is_line_start = true;
                is_hash_line = false;
            } else {
                is_line_start = false;
            }

            if (is_in_string) {
                struct msgenc_charmap_entry* entry_match = NULL;
                for (size_t map_i = 0; map_i < charmap.map_len; map_i++) {
                    struct msgenc_charmap_entry* entry = &charmap.map[map_i];

                    if (i + entry->string_sz > avail) {
                        // process_end excluding the last MAP_STRING_SZ_MAX bytes unless EOF
                        // means all strings have enough bytes to compare against at buffer+i
                        // (unless EOF)
                        assert(is_eof);
                        continue;
                    }

                    if (memcmp(buffer + i, entry->string, entry->string_sz) == 0) {
                        entry_match = entry;
                        break;
                    }
                }
                if (entry_match != NULL) {
                    // write buffer[processed_end:i]
                    if (processed_end != i) {
                        assert(processed_end < i);
                        size_t nwritten = fwrite(buffer + processed_end, i - processed_end, 1, f_out);
                        if (nwritten != 1) {
                            perror("fwrite");
                            util_fatal_error("msgenc_process: fwrite error");
                        }
                    }

                    // output the replacement byte value
                    fputc(entry_match->value, f_out);

                    i += entry_match->string_sz;
                    processed_end = i;

                    // otherwise eg the \ in "\"\\n\"" (the 4 characters string `"\n"`) counts as escaping the closing "
                    n_backslashes = 0;
                } else {
                    i++;
                }
            } else {
                i++;
            }
        }

        // write buffer[processed_end:process_end]
        if (processed_end != process_end) {
            assert(processed_end < process_end);
            size_t nwritten = fwrite(buffer + processed_end, process_end - processed_end, 1, f_out);
            if (nwritten != 1) {
                perror("fwrite");
                util_fatal_error("msgenc_process: fwrite error");
            }
        }
        // move buffer[process_end:] to buffer[0:]
        memmove(buffer, buffer + process_end, avail - process_end);
        avail = avail - process_end;
    }
    free(buffer);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path/to/charmap.txt>\n", argv[0]);
        fprintf(stderr, "Reads from stdin and writes to stdout.\n");
        return EXIT_FAILURE;
    }

    struct msgenc_charmap charmap = msgenc_charmap_read(argv[1]);

    if (0) {
        // dump the charmap (for debugging)
        for (size_t i = 0; i < charmap.map_len; i++) {
            struct msgenc_charmap_entry* entry = &charmap.map[i];
            fprintf(stderr, "[%zd] \"%s\" -> 0x%02" PRIX8 "\n", i, entry->string, entry->value);
        }
    }

    msgenc_process(stdin, stdout, charmap);

    msgenc_charmap_free(&charmap);

    return EXIT_SUCCESS;
}

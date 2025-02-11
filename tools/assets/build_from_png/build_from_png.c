#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../n64texconv/src/libn64texconv/n64texconv.h"

#define NUM_FORMATS 9
static const struct fmt_info {
    const char* name;
    int fmt;
    int siz;
} fmt_map[NUM_FORMATS] = {
    // clang-format off
    { "i4",     G_IM_FMT_I,    G_IM_SIZ_4b,  },
    { "i8",     G_IM_FMT_I,    G_IM_SIZ_8b,  },
    { "ci4",    G_IM_FMT_CI,   G_IM_SIZ_4b,  },
    { "ci8",    G_IM_FMT_CI,   G_IM_SIZ_8b,  },
    { "ia4",    G_IM_FMT_IA,   G_IM_SIZ_4b,  },
    { "ia8",    G_IM_FMT_IA,   G_IM_SIZ_8b,  },
    { "ia16",   G_IM_FMT_IA,   G_IM_SIZ_16b, },
    { "rgba16", G_IM_FMT_RGBA, G_IM_SIZ_16b, },
    { "rgba32", G_IM_FMT_RGBA, G_IM_SIZ_32b, },
    // clang-format on
};

#define strequ(s1, s2) (strcmp(s1, s2) == 0)
#define strstartswith(s, prefix) (strncmp(s, prefix, strlen(prefix)) == 0)
bool strendswith(const char* s, const char* suffix) {
    size_t len_s = strlen(s);
    size_t len_suffix = strlen(suffix);
    return (len_s >= len_suffix) && (strncmp(s + len_s - len_suffix, suffix, len_suffix) == 0);
}

bool parse_png_p(char* png_p_buf, const struct fmt_info** fmtp, int* elem_sizep, char** tlut_namep,
                 int* tlut_elem_sizep) {
    // The last 4 (or less) suffixes, without the '.'
    const int max_n_suffixes = 4;
    char* png_p_suffixes[max_n_suffixes];
    int n_suffixes_found = 0;
    for (size_t i = strlen(png_p_buf) - 1; i >= 0; i--) {
        if (png_p_buf[i] == '.') {
            png_p_suffixes[n_suffixes_found] = &png_p_buf[i + 1];
            n_suffixes_found++;
            if (n_suffixes_found >= max_n_suffixes) {
                break;
            }
            png_p_buf[i] = '\0';
        }
    }

    if (n_suffixes_found == 0 || !strequ(png_p_suffixes[0], "png")) {
        fprintf(stderr, "png path doesn't end with .png\n");
        return false;
    }
    int i_suffix = 1;
    int i_suffix_elemtype = -1;
    int i_suffix_tlut = -1;
    int i_suffix_fmt = -1;
    if (i_suffix < n_suffixes_found &&
        (strequ(png_p_suffixes[i_suffix], "u32") || strequ(png_p_suffixes[i_suffix], "u64"))) {
        i_suffix_elemtype = i_suffix;
        i_suffix++;
    }
    if (i_suffix < n_suffixes_found && strstartswith(png_p_suffixes[i_suffix], "tlut_")) {
        i_suffix_tlut = i_suffix;
        i_suffix++;
    }
    if (i_suffix >= n_suffixes_found) {
        fprintf(stderr, "png path is missing a .format suffix\n");
        return false;
    }
    i_suffix_fmt = i_suffix;

    if (i_suffix_elemtype < 0) {
        fprintf(stderr, "png path is missing a .u32 or .u64 suffix\n");
        return false;
    } else {
        if (strequ(png_p_suffixes[i_suffix_elemtype], "u64")) {
            *elem_sizep = 8;
        } else if (strequ(png_p_suffixes[i_suffix_elemtype], "u32")) {
            *elem_sizep = 4;
        } else {
            // unreachable
            assert(false);
        }
    }

    const struct fmt_info* fmt = NULL;

    for (size_t i = 0; i < NUM_FORMATS; i++) {
        if (strequ(fmt_map[i].name, png_p_suffixes[i_suffix_fmt])) {
            fmt = &fmt_map[i];
        }
    }

    if (fmt == NULL) {
        fprintf(stderr, "png path is missing a .format suffix\n");
        return false;
    }

    if (fmt->fmt == G_IM_FMT_CI && i_suffix_tlut >= 0) {
        if (strendswith(png_p_suffixes[i_suffix_tlut], "_u64")) {
            *tlut_elem_sizep = 8;
        } else if (strendswith(png_p_suffixes[i_suffix_tlut], "_u32")) {
            *tlut_elem_sizep = 4;
        } else {
            fprintf(stderr, "png path with ci format has a .tlut_ suffix without a _u32 or _u64 suffix\n");
            return false;
        }
        // extract "ABC" from the "tlut_ABC_uXX" suffix
        if (strlen(png_p_suffixes[i_suffix_tlut]) <= strlen("tlut__uXX")) {
            fprintf(stderr, "png path with ci format has a bad .tlut_ suffix\n");
            return false;
        }
        png_p_suffixes[i_suffix_tlut][strlen(png_p_suffixes[i_suffix_tlut]) - strlen("_uXX")] = '\0';
        *tlut_namep = strdup(png_p_suffixes[i_suffix_tlut] + strlen("tlut_"));
    }

    *fmtp = fmt;

    return true;
}

bool write_bytes(const char* path, void* data, size_t nbytes) {
    FILE* f = fopen(path, "wb");
    if (f == NULL) {
        perror("fopen");
        fprintf(stderr, "Can't open for writing: %s\n", path);
        return false;
    }
    if (fwrite(data, nbytes, 1, f) != 1) {
        perror("fwrite");
        return false;
    }
    if (fclose(f) != 0) {
        perror("fclose");
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc != 3) {
    usage:
        fprintf(stderr, "Usage: build_from_png path/to/file.png path/to/file.bin\n");
        fprintf(stderr, "The png file should be named like:\n");
        fprintf(stderr, " - texName.format[.u32|.u64].png (non-ci formats)\n");
        fprintf(stderr, " - texName.ci[4|8].tlut_tlutName[_u32|_u64][.u32|.u64].png\n");
        return EXIT_FAILURE;
    }
    const char* png_p = argv[1];
    const char* bin_p = argv[2];

    const struct fmt_info* fmt;
    int elem_size;
    char* tlut_name = NULL;
    int tlut_elem_size = -1;

    {
        char* png_p_buf = strdup(png_p);
        bool success = parse_png_p(png_p_buf, &fmt, &elem_size, &tlut_name, &tlut_elem_size);
        free(png_p_buf);
        if (!success) {
            goto usage;
        }
    }

    struct n64_image* img = n64texconv_image_from_png(png_p, fmt->fmt, fmt->siz, G_IM_FMT_RGBA);

    void* img_bin = n64texconv_image_to_bin(img, false, false);
    size_t nbytes = texel_size_bytes(img->width * img->height, img->siz);
    if (!write_bytes(bin_p, img_bin, nbytes)) {
        free(tlut_name);
        free(img_bin);
        n64texconv_image_free(img);
        return EXIT_FAILURE;
    }

    if (fmt->fmt == G_IM_FMT_CI) {
        // TODO ...
        n64texconv_palette_to_bin(img->pal, false);
    }

    free(tlut_name);
    free(img_bin);
    n64texconv_image_free(img);
    return EXIT_SUCCESS;
}

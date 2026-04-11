#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spec.h"
#include "util.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s path/to/spec segment_name\n", argv[0]);
        fprintf(stderr, "Writes a segment's includes to stdout\n");
        return EXIT_FAILURE;
    }

    char* spec_p = argv[1];
    char* segment_name = argv[2];

    char* spec = util_read_whole_file(spec_p, NULL);
    struct Segment* segments;
    int segment_count;
    parse_rom_spec(spec, &segments, &segment_count);

    int segment_i = -1;
    for (int i = 0; i < segment_count; i++) {
        if (strcmp(segments[i].name, segment_name) == 0) {
            segment_i = i;
        }
    }

    int ret = EXIT_SUCCESS;

    if (segment_i < 0) {
        fprintf(stderr, "Segment %s not found\n", segment_name);
        ret = EXIT_FAILURE;
    } else {
        for (int i = 0; i < segments[segment_i].includesCount; i++) {
            printf("%s\n", segments[segment_i].includes[i].fpath);
        }
    }

    free_rom_spec(segments, segment_count);
    free(spec);
    return ret;
}

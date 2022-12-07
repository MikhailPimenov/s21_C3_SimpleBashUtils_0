#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_getline.h"
#include <assert.h>
ssize_t my_getline_allocate(char **line, size_t *allocated_size, FILE *stream) {
    size_t pos;
    int c;

    if (line == NULL || stream == NULL || allocated_size == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*line == NULL) {
        *line = malloc(128);
        if (*line == NULL) {
            return -1;
        }
        *allocated_size = 128;
    }

    pos = 0;
    while (c != EOF) {
        if (pos + 1 >= *allocated_size) {
            size_t new_size = *allocated_size + (*allocated_size >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*line, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *allocated_size = new_size;
            *line = new_ptr;
        }

        ((unsigned char *)(*line))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*line)[pos] = '\0';
    return pos;
}

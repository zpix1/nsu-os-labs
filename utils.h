#ifndef H_LABS_UTILS
#define H_LABS_UTILS

#include <errno.h>
#include <stdlib.h>

#define DO_OR_DIE(x, s) do { \
        if ((x) < 0) { \
                    perror(s); \
                    exit(1);   \
                } \
} while (0)
#endif
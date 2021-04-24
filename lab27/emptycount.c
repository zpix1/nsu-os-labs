#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <libgen.h>

#define BUFSIZE 256

#define DO_OR_DIE(x, s) do { \
    if ((x) < 0) { \
        perror(s); \
        exit(1);   \
    } \
} while (0)

// cat foo.pl | sed '/^\s*#/d;/^\s*$/d' | wc -l

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s requires exactly one argument, filename\n", argv[0]);
        return 1;
    }

    FILE *file;
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
        return 1;
    }

    // FILE* pipe1[2];
    FILE* pipe2[2];

    // DO_OR_DIE(p2open("sed '/^\\s*#/d;/^\\s*$/d'", pipe1), "p2open sed");
    DO_OR_DIE(p2open("wc -l", pipe2), "p2open wc");

    char line[BUFSIZE];

    while (1) {
        if (fgets(line, BUFSIZE, file) == NULL) {
            break;
        }
        if (line[0] == '\n' || line[0] == '\0') {
            fputs(line, pipe2[0]);
        }
    }
    
    fclose(pipe2[0]);

    if (fgets(line, BUFSIZE, pipe2[1]) == NULL) {
       fprintf(stderr, "fgets wc -l failed\n");
    }
    fputs(line, stdout);

    return 0;
}

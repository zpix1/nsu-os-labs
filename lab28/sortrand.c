#include <stdio.h>
#include <libgen.h>

#include "../utils.h"

#define BUFSIZE 256

// python3 -c "from random import randint; print('\n'.join(map(str, [randint(0, 99) for _ in range(100)])))" | sort -n | paste -s -d"\t\t\t\t\t\t\t\\t\\t\n" -

int main() {
    FILE* fds[2];
    
    DO_OR_DIE(p2open("/bin/sort -n", fds), "p2open");

    for (int i = 0; i < 100; i++) {
        fprintf(fds[0], "%d\n", rand() % 100);
    }

    fclose(fds[0]);

    char buf[BUFSIZE];
    
    int i = 0;
    int v;
    while (fscanf(fds[1], "%d", &v) > 0) {
        printf("%d ", v);
        if (++i % 10 == 0) {
            printf("\n");
        }
    }
    return 0;
}

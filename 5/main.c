#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 512
#define ENTRYSTEP 10

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("no filename specified");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }
    int entrymaxn = 100;

    off_t total = 0;
    ssize_t scanned;
    char buf[BUFSIZE];
    struct LineEntry {
        off_t offset;
        ssize_t length;
    };

    struct LineEntry* entries = malloc(entrymaxn * sizeof(struct LineEntry));
    entries[0].offset = 0;
    int entryno = 0;
    while (scanned = read(fd, buf, BUFSIZE)) {
        for (ssize_t i = 0; i < scanned; i++) {
            if ((buf[i] == '\n') 
                    || (scanned < BUFSIZE && i == scanned - 1)) {
                entries[entryno].length = total + i - entries[entryno].offset;
                entryno++;
                entries[entryno].offset = total + i + 1;
            }
            if (entryno == entrymaxn) {
                entrymaxn += ENTRYSTEP;
                entries = realloc(entries, entrymaxn * sizeof(struct LineEntry));
            }
        }
        total += scanned;
    }

    for (int i = 0; i < entryno; i++) {
        printf("offt: %d len: %d\n", entries[i].offset, entries[i].length);
    }

    int linen;
    while (1) {
        printf("Enter line index (or zero to exit): ");
        scanf("%d", &linen);
        if (linen == 0) {
            break;
        }
        if (linen > entryno || linen < 0) {
            printf("invalid line index\n");
        } else {
            linen--;
            lseek(fd, entries[linen].offset, SEEK_SET);
            char* lbuf = malloc(entries[linen].length + 1);
            read(fd, lbuf, entries[linen].length);
            lbuf[entries[linen].length] = '\0';
            printf("%s\n", lbuf);
            free(lbuf);
        }
    };

    free(entries);
}

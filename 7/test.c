#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    // int fd = open("/dev/zero", O_RDWR); 
    char* a = mmap(0x1337000, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (a == -1) {
        perror("mmap");
        return 0;
    }
    a[0] = 'p';
    if (fork() == 0) {
        printf("CHILD: setting a[0] to 'c'\n");
        a[0] = 'c';
    } else {
        while (1) {
            printf("PARENT: a[0]: %c\n", a[0]);
            sleep(1);
        }
    }
}
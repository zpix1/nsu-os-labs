#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <ulimit.h>

int main(int argc, char **argv, char **envp) {
    printf("uid: %d, euid: %d\n", getuid(), geteuid());
    FILE* f = fopen("file", "r");
    if (f == NULL) {
        perror("file error");
    } else {
        printf("file opened\n");
        fclose(f);
    }

    setuid(getuid());
    printf("uid: %d, euid: %d\n", getuid(), geteuid());
    f = fopen("file", "r");
    if (f == NULL) {
        perror("file setuid(getuid()) open error");
    } else {
        printf("file2 setuid(getuid()) opened\n");
        fclose(f);
    }

    return 0; 
}

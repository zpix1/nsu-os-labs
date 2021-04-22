#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main() {
    DIR* dirp;
    struct dirent *dp;

    if ((dirp = opendir(".")) == NULL) {
        perror(".");
        return 1;
    }

    while ((dp = readdir(dirp)) != NULL) {
        printf("%s\n", dp->d_name);
    }

    return 1;
}

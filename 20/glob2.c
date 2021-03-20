#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <glob.h>

int main() {
    char glob_str[PATH_MAX + 1];
    printf("Enter glob pattern: ");
    if (scanf("%1024[^\n]%*c", glob_str) != 1) {
        fprintf(stderr, "glob input error\n");
        return 1;
    }

    glob_t paths;
    int retval;
    
    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    if (glob(glob_str, GLOB_NOCHECK | GLOB_NOSORT, NULL, &paths) == 0) {
        for (int i = 0; i < paths.gl_pathc; i++) {
            printf("%s\n", paths.gl_pathv[i]);
        }
        
        globfree(&paths);
    } else {
        perror("glob() failed");
    }
    return 0;
}

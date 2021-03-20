#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <glob.h>

#define V2

bool glob_match(char* pat, char* str) {
    /*
     * Backtrack to previous * on mismatch and retry starting one
     * character later in the string.  Because * matches all characters
     * (no exception for /), it can be easily proved that there's
     * never a need to backtrack multiple levels.
     */
    char *back_pat = NULL, *back_str = back_str;

    /*
     * Loop over each token (character or class) in pat, matching
     * it against the remaining unmatched tail of str.  Return false
     * on mismatch, or true after matching the trailing nul bytes.
     */
    for (;;) {
        unsigned char c = *str++;
        unsigned char d = *pat++;

        switch (d) {
        case '?':	/* Wildcard: anything but nul */
            if (c == '\0')
                return false;
            break;
        case '*':	/* Any-length wildcard */
            if (*pat == '\0')	/* Optimize trailing * case */
                return true;
            back_pat = pat;
            back_str = --str;	/* Allow zero-length match */
            break;
        default:	/* Literal character */
            if (c == d) {
                if (d == '\0')
                    return true;
                break;
            }
            if (c == '\0' || !back_pat)
                return false;	/* No point continuing */
            /* Try again from last *, one character later in str. */
            pat = back_pat;
            str = ++back_str;
            break;
        }
    }
}

int main() {
    char glob_str[PATH_MAX + 1];
    printf("Enter glob pattern: ");
    if (scanf("%1024[^\n]%*c", glob_str) != 1) {
        fprintf(stderr, "glob input error\n");
        return 1;
    }
    
    // for (int i = 0; glob_str[i] != '\0'; i++) {
    //     if (glob_str[i] == '/') {
    //         fprintf(stderr, "/ is banned\n");
    //         return 1;
    //     }
    // }

#ifdef V2
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
#else
    DIR* dirp;
    struct dirent *dp;

    if ((dirp = opendir(".")) == NULL) {
        perror(".");
        return 1;
    }

    int cnt = 0;

    while ((dp = readdir(dirp)) != NULL) {
        if (glob_match(glob_str, dp->d_name)) {
            printf("%s\n", dp->d_name);
            cnt++;
        }
    }

    if (cnt == 0) {
        printf("No matches found for %s\n", glob_str);
    }
    closedir(dirp);
#endif
    return 0;
}

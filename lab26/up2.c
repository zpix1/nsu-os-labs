#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#define BUFSIZE 256

int main() {
    FILE *fin, *fout;
    char line[BUFSIZE];
    fin = popen("./writer", "r");
    if (fin == NULL) {
        perror("popen ./writer");
        return 1;
    }

    fout = popen("./upper", "w");
    if (fout == NULL) {
        perror("popen ./upper");
        return 1;
    }

    while (fgets(line, BUFSIZE, fin)) {
        fputs(line, fout);
    }

    pclose(fin);
    pclose(fout);
}
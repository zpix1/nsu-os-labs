#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#define BUFSIZE 256

int main() {
    char line[BUFSIZE];
    
    while (fgets(line, BUFSIZE, stdin) != NULL) {
        int i = 0;
        while (line[i] != '\0') {
            line[i] = toupper(line[i]);
            i++;
        }
        fputs(line, stdout);
    }

    return 0;
}
#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

extern char **environ;
void print(char** v) {
    while (*v != NULL) {
        printf("%s\n", *v);
        v++;
    }
}

char** d;

void printenvpenviron() {
    printf("envp:\n");
    print(d);

    printf("environ:\n");
    print(environ);
    printf("\n");
}

int main(int argc, char** argv, char** envp) {
    d = envp;

    printf("=== start\n");
    printenvpenviron();

    // printf("=== putenv PUTENV_KEK=MEM\n");
    // putenv("PUTENV_KEK=MEM");
    // printenvpenviron();

    printf("=== setenv SECOND=CHANGED\n");
    setenv("SECOND", "CHANGED", 1);
    printenvpenviron();

    printf("=== change setenv ROFL to CHANGED\n");
    setenv("ROFL", "CHANGED", 1);
    printenvpenviron();

    // printf("=== change putenv ROFL to CHANGED\n");
    // putenv("ROFL=CHANGED");
    // printenvpenviron();
}
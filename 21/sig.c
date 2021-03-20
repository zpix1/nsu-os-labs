#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int count = 0;

void intcatch() {
    printf("\a");
    count++;
    signal(SIGINT, intcatch);
}

void quitcatch() {
    printf("\nbeeped %d times\n", count);
    exit(0);
}

int main() {
    signal(SIGINT, intcatch);
    signal(SIGQUIT, quitcatch);
    for (;;);
    return 0;
}

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;
int end = 0;

void intcatch() {
    write(1, "\a", 1);
    count++;
}

void quitcatch() {
    end = 1;
}

int main() {
    // old, but portable (bc in C standard)
    // btw windows signals? they work differently
    // signal(SIGINT, intcatch);
    // signal(SIGQUIT, quitcatch);

    // marked obsolescent
    // blocks signals while in handler
    // sigset(SIGINT, intcatch);
    // sigset(SIGQUIT, quitcatch);

    // sigaction is preferred
    // previous action should be examined before setting signals, 
    // but here we can skip that check
    struct sigaction int_action;
    int_action.sa_sigaction = intcatch;
    sigemptyset(&int_action.sa_mask);
    int_action.sa_flags = 0;
    sigaction(SIGINT, &int_action, NULL);

    struct sigaction quit_action;
    quit_action.sa_sigaction = quitcatch;
    sigemptyset(&quit_action.sa_mask);
    quit_action.sa_flags = 0;
    sigaction(SIGQUIT, &quit_action, NULL);

    for (;;) {
        pause();
        if (end) {
            // should write Interrupted
            perror("wait exited");
            printf("\nbeeped %d times\n", count);
            break;
        }
    }
    return 0;
}

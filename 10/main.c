#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char** argv) {
    int status;
    pid_t pid;
    if ((pid = fork()) != -1) {
        if (pid == 0) {
            execvp(argv[1], &argv[1]);
            perror(argv[1]);
            return 1;
        } else {
            int stat;
            pid_t res_pid = wait(&stat);
            if (res_pid == -1) {
                perror("wait");
                return 1;
            }
            if (WIFEXITED(stat)) {
                printf("%s exited with code %d\n", argv[1], WEXITSTATUS(stat));
            } else if (WIFSIGNALED(stat)) {
                printf("%s signaled with signal %d\n", WTERMSIG(stat));
            } else {
                printf("strange...\n");
            }
        }
    } else {
        perror("fork");
    }
    return 0;
}
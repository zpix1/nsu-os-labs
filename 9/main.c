#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    pid_t pid;
    if ((pid = fork()) != -1) {
        if (pid == 0) {
            execlp("cat", "cat", "/etc/passwd", 0);
            perror("execlp");
        } else {
            if (wait(0) == -1) {
                perror("wait");
                return 1;
            }
            printf("it is from parent process\nhi hi hi hi\nhello hello hello\n");
        }
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}
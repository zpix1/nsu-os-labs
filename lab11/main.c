#include <unistd.h>
#include <stdlib.h>

extern char** environ;

int execvpe(const char *file, char *const argv[], char *const envp[]) {
    char** oldenv = environ;
    environ = envp;
    int ret = execvp(file, argv);
    environ = oldenv;
    return ret;
}

int main() {
    char* args[] = {
        "env",
        0
    };
    char* env[] = {
        "kek=MEM",
        0
    };
    return execvpe("env", args, env);
}

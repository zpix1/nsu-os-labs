#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

void print_help() {
    printf("this program is useless, so just don't use it\n");
}

#define COMMAND_SIZE 200

int main(int argc, char **argv, char **envp) { 
    if (argc == 1) {
        print_help();
        return 0;
    }

    int opt;
    unsigned long int value;
    struct rlimit limit;
    char cwd[PATH_MAX];
    char command[COMMAND_SIZE];
    char* endptr;
    
    while ((opt = getopt(argc, argv, "ispuU:cC:dvV:h")) != -1) {
        switch (opt) {
            case 'i':
                // system("id");
                printf("uid: %d, euid: %d, guid: %d, guieid: %d\n", getuid(), geteuid(), getgid(), getegid());
                break;
            case 's':
                printf("became group leader with result %d\n", setpgid(0, 0));
                break;
            case 'p':
                printf("pid: %d; ppid: %d; pgid: %d", getpid(), getppid(), getpgrp());
                break;
            case 'u':
                // system("ulimit");
                if (getrlimit(RLIMIT_NOFILE, &limit) != 0) {
                    perror("getrlimit() error");
                    return 1;
                }
                printf("ulimit: %lu\n", limit.rlim_cur);
                break;
            case 'U':
                if (getrlimit(RLIMIT_NOFILE, &limit) != 0) {
                    perror("getrlimit() error");
                    return 1;
                }
                limit.rlim_cur = strtoul(optarg, &endptr, 10);
                if (setrlimit(RLIMIT_NOFILE, &limit) != 0) {
                    perror("setrlimit() error");
                    return 1;
                }
                printf("ulimit set to %ld\n", limit.rlim_cur);
                break;
            case 'c':
                if (getrlimit(RLIMIT_CORE, &limit) != 0) {
                    perror("getrlimit() error");
                    return 1;
                }
                printf("core: %lu\n", limit.rlim_cur);
                break;
            case 'C':
                if (getrlimit(RLIMIT_CORE, &limit) != 0) {
                    perror("getrlimit() error");
                    return 1;
                }
                limit.rlim_cur = strtoul(optarg, &endptr, 10);
                if (setrlimit(RLIMIT_CORE, &limit) != 0) {
                    perror("setrlimit() error");
                    return 1;
                }
                printf("core set to %ld\n", limit.rlim_cur);
                break;
            case 'd':
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("current working dir: %s\n", cwd);
                } else {
                    perror("getcwd() error");
                    return 1;
                }
                break;
            case 'v':
                printf("printing env variables:\n");
                system("env");
                break;
            case 'V':
                putenv(optarg);
                printf("set %s\n", optarg);
                break;
            case 'h':
                print_help();
                break;
            // case '?':
            //     printf("unknown option: %c\n", optopt); 
            //     break;
        }
    }
    for(; optind < argc; optind++){
        printf("extra arguments: %s\n", argv[optind]);
    }
    return 0; 
}
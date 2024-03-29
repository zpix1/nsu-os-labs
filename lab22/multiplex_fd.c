#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define BUFSIZE 256
#define TIME_OUT 5

enum state {
    STATE_EOF,
    STATE_ON,
    STATE_INV
};

struct mtype {
    int fd;
    char* fname;
    struct mtype* next;
    struct mtype* prev;
};

void sigcatch() {}

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "wrong amount of arguments\nusage: multiplex <file1> <file2> ...\n");
        return 1;
    }

    struct sigaction alarm_action;
    alarm_action.sa_sigaction = sigcatch;
    sigemptyset(&alarm_action.sa_mask);
    alarm_action.sa_flags = 0;
    sigaction(SIGALRM, &alarm_action, NULL);


    const int n = argc - 1;
    
    struct mtype* head = NULL;
    struct mtype* current = NULL;

    for (int i = 0; i < n; i++) {
        int fd = open(argv[i + 1], O_RDONLY);
        if (fd == -1) {
            perror(argv[i + 1]);
        } else {
            struct mtype* new = malloc(sizeof(struct mtype));
            new->fd = fd;
            new->prev = current;
            new->next = NULL;
            new->fname = argv[i +1 ];
            if (current == NULL) {
                head = new;
            } else {
                current->next = new;
            }
            current = new;
            printf("%s (%d) opened\n", new->fname, new->fd);
        }
    }
    
    if (head == NULL) {
        fprintf(stderr, "no valid files given...\n");
        exit(1);
    }

    char buf[BUFSIZE + 1];

    current->next = head;
    head->prev = current;

    current = head;
    while (current->next != NULL) {
        alarm(TIME_OUT);
        printf("reading from %s (%d)...\n", current->fname, current->fd);
        int res = read(current->fd, buf, BUFSIZE);
        if (res == -1 || res == 0) {
            if (errno == EINTR) {
                printf("timeout\n");
            } else {
                current->next->prev = current->prev;
                current->prev->next = current->next;
                close(current->fd);
                printf("removed %d\n", current->fd);
                struct mtype* tmp = current->next;
                free(current);
                if (tmp == current) {
                    break;
                }
                current = tmp;
            }
        } else {
            buf[res] = '\0';
            printf("read: %s", buf);
            current = current->next;
        }
    }
}

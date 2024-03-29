#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 256
#define TIMEOUT 5

enum state {
    STATE_EOF,
    STATE_ON,
    STATE_INV
};

struct mtype {
    int fd;
    FILE* file;
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
        FILE* file = fopen(argv[i + 1], "rb");
        if (file == NULL) {
            perror(argv[i + 1]);
        } else {
            struct mtype* new = malloc(sizeof(struct mtype));
            new->file = file;
            new->prev = current;
            new->next = NULL;
            new->fname = argv[i + 1];
            if (current == NULL) {
                head = new;
            } else {
                current->next = new;
            }
            current = new;
            printf("%s opened\n", new->fname);
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
        alarm(TIMEOUT);
        printf("reading from %s...\n", current->fname);
        errno = 0;
        char* res = fgets(buf, BUFSIZE, current->file);
        // int res = read(current->fd, buf, BUFSIZE);
        if (res == NULL) {
            if (errno == EINTR) {
                printf("timeout\n");
                current = current->next;
            } else if (errno != 0) {
                perror(current->fname);
            } else {
                current->next->prev = current->prev;
                current->prev->next = current->next;
                fclose(current->file);
                printf("removed %s\n", current->fname);
                struct mtype* tmp = current->next;
                free(current);
                if (tmp == current) {
                    break;
                }
                current = tmp;
            }
        } else {
            buf[strlen(buf) - 1] = '\0';
            printf("read: %s\n", buf);
            current = current->next;
        }
    }
}

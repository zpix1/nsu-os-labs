#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SEND_SZ 300
static const long Done = 99L;

int main() {
    key_t key;
    int mid;
    struct msgbuf {
        long mtype;
        char mtext[40];
    } buf;

    static char *strings[3] = {
        "hello",
        "how are you",
        "good-bye"
    };

    int i, rtn;

    if ((key = ftok(".", 'a')) == -1) {
        perror("Can't form key");
        exit(1);
    }

    mid = msgget(key, IPC_CREAT | 0660);

    if (mid == -1) {
        perror("Sender can't make msg queue");
        exit(2);
    }

    buf.mtype = 1L;

    for (i = 0; i < 3; i++) {
        strcpy(buf.mtext, strings[i]);
        if (msgsnd(mid, &buf, strlen(buf.mtext) + 1, 0)) {
            perror("Sender can't msgsnd");
            exit(3);
        }
    }

    rtn = msgrcv(mid, &buf, MAX_SEND_SZ, Done, 0);
    

    if (rtn == -1) {
        perror("Sender can't msgrcv");
        exit(4);
    }

    msgctl(mid, IPC_RMID, NULL);

    return 0;
}
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX_RCV_SZ 300
static const long Fifo = 0L;
static const long Done = 99L;

int main() {
    struct msgbuf {
        long mtype;
        char mtext[MAX_RCV_SZ];
    } buf;

    key_t key;
    int mid, rtn;
    if ((key = ftok(".", 'a')) == -1){
        perror("Can't form key");
        exit(1);
    }

    mid = msgget(key, 0);
    if (mid == -1) {
        perror("Receiver can't access msg queue");
        exit(2);
    }

    while(1) {
        rtn = msgrcv(mid, &buf, MAX_RCV_SZ, Fifo, 0);

        printf("rtn=%d buf.mtype=%ld buf.mtext=%s\n", rtn, buf.mtype, buf.mtext);

        if (!strcmp(buf.mtext, "good-bye")) {
            break;
        }
    }
    buf.mtype = Done;
    msgsnd(mid, &buf, 0, 0);
    return 0;
}
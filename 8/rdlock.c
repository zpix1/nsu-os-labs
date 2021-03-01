#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char** argv) {
    char* fname = "file";
    int fd;
    if ((fd = open(fname, O_RDWR)) == -1) {
        perror(fname);
        exit(1);
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("can't lock");
        exit(1);
    }

    system("nano file");

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return 0;
}

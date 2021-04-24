#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define MAX_REQ_SIZE 1000
#define MAX_URL_SIZE 256
#define POLL_TIMEOUT 10000
#define LINES_PER_SCREEN 26
#define BUF_SIZE 1000000
#define PORT 80
#define BODY_DELIM "\r\n\r\n"

struct termios save;
int tios_saved = 0;
int tty = STDIN_FILENO;

void restore_term() {
    write(1, "bye!\n", 6);
    if (tcsetattr(tty, TCSANOW, &save) == -1) {
        perror("tcsetattr");
    }
}

void sigint_handle() {
    restore_term();
    exit(1);
}

int print_http_contents(char* full_url) {
    char host[MAX_URL_SIZE];
    char url[MAX_URL_SIZE];

    int delim_idx;

    for (delim_idx = 0; full_url[delim_idx] != '\0'; delim_idx++) {
        if (full_url[delim_idx] == '/' || delim_idx > MAX_URL_SIZE) {
            break;
        }
    }
    if (delim_idx > MAX_URL_SIZE) {
        return -1;
    }

    if (full_url[delim_idx] == '\0') {
        strcpy(host, full_url);
        strcpy(url, "");
    } else {
        strcpy(url, full_url + delim_idx);
        full_url[delim_idx] = '\0';
        strcpy(host, full_url);
    }

    printf("host: %s, url: %s\n", host, url);

    char *message_fmt = "GET %s HTTP/1.0\r\n\r\n";

    char message[MAX_REQ_SIZE], response[BUF_SIZE];

    snprintf(message, MAX_REQ_SIZE, message_fmt, url);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    // struct timeval timeout;      
    // timeout.tv_sec = 10;
    // timeout.tv_usec = 0;
                
    // if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
    //     perror("setsockopt timeout");
    // }

    // int syn_retries = 2;
    // setsockopt(sockfd, IPPROTO_TCP, TCP_SYNCNT, &syn_retries, sizeof(syn_retries));

                        
    printf("gethostbyname now\n");
    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        perror("gethostbyname");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    printf("connect now\n");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    int w = write(sockfd, message, strlen(message));
    if (w == -1) {
        perror("write");
        close(sockfd);
        return -1;
    }

    struct pollfd fds[2] = {
        {.fd = sockfd, .events = POLLIN | POLLHUP, .revents = 0},
        {.fd = tty, .events = POLLIN, .revents = 0},
    };

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    int lines_written = 0;
    int total_read = 0;
    int total_written = 0;
    int head_skipped = 0;

    while (1) {
        fds[0].revents = 0;
        fds[1].revents = 0;

        int pollres = poll(fds, 2, POLL_TIMEOUT);

        if (pollres == 0) {
            continue;
        }

        if (pollres == -1) {
            perror("poll");
            return -1;
        }

        if ((fds[0].revents & POLLHUP) != 0) {
            printf("POLLHUP\n");
            fds[0].events = 0;
        }

        if ((fds[0].revents & POLLIN) != 0) {
            ssize_t r = read(sockfd, response + total_read, BUF_SIZE - total_read);
            if (r == 0) {
                fds[0].events = 0;
            }
            if (r == -1) {
                perror("read socket");
                return -1;
            }

            total_read += r;
        }
        
        if ((fds[1].revents & POLLIN) != 0) {
            char c;
            read(tty, &c, 1);
            if (c == ' ') {
                lines_written = 0;
                fds[1].revents = 0;
            }
            if (c == 'q') {
                printf("exiting...\n");
                return 0;
            }
        }

        // minor bug here
        if (lines_written <= LINES_PER_SCREEN) {
            if (!head_skipped) {
                char* delim_pos = strstr(response, BODY_DELIM);
                if (delim_pos != NULL) {
                    total_written = delim_pos - response + strlen(BODY_DELIM);
                    head_skipped = 1;
                }
            }
            if (head_skipped) {
                int tpos;
                for (tpos = total_written; tpos < total_read; tpos++) {
                    if (response[tpos] == '\n' || response[tpos] == '\r') {
                        lines_written++;
                        putchar('\n');
                    } else {
                        putchar(response[tpos]);
                    }
                    if (lines_written > LINES_PER_SCREEN) {
                        printf("Press space to scroll down\n");
                        tpos++;
                        break;
                    }
                }
                total_written = tpos;
            }
        }

        if (total_read == total_written && fds[0].events == 0) {
            break;
        }
    }

    close(sockfd);

    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("wrong amount of args\nusage: %s <url>\n", argv[0]);
        exit(1);
    }

    if (isatty(tty) == 0) {
        printf("you are not a terminal, i wont serve for you\n");
        exit(1);
    }

    // Set terminal mode
    struct termios tp;
    if (tcgetattr(tty, &tp) == -1) {
        perror("tcgetattr");
    }

    save = tp;
    tp.c_lflag &= ~ECHO;
    tp.c_lflag &= ~ICANON;
    tp.c_cc[VMIN] = 1;

    if (tcsetattr(tty, TCSAFLUSH, &tp) == -1) {
        perror("tcsetattr");
    }

    // Add CTRL-C handler
    struct sigaction sa;
    sa.sa_handler = sigint_handle;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    print_http_contents(argv[1]);

    restore_term();

    return 0;
}

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];
int main() {
    time_t now;
    struct tm *sp;
    time(&now);
    putenv("TZ=America/Los_Angeles");
    // now -= 60 * 60 * (7 + 8);
    printf("%s", ctime(&now));

    // sp = localtime(&now);
    // printf("%d/%d/%d %d:%02d %s\n",
    //      sp->tm_mon + 1, sp->tm_mday,
    //      sp->tm_year, sp->tm_hour,
    //      sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}

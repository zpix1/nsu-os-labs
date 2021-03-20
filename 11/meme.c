#include <stdlib.h>
#include <stdio.h>

int main() {
    long* a = malloc(2*sizeof(long));
    long* b = malloc(2*sizeof(long));
    printf("a: %p\nb: %p\n", a, b);
    free(a);
    free(b);
    //a[0] = 1;
    //a[1] = 1;
    free(b);
    printf("malloc(): %p\n", malloc(2*sizeof(long)));
    printf("malloc(): %p\n", malloc(2*sizeof(long)));
}

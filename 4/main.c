#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_SIZE 300

struct Entry {
    char* str;
    struct Entry* next;
};

void check(void* ptr) {
    if (ptr == NULL) {
        perror("very bad");
        exit(1);
    }
}

int main() {
    char str[MAX_STR_SIZE + 1];
    struct Entry* head = malloc(sizeof(struct Entry));
    check(head);
    head->next = NULL;

    struct Entry* next = head;
    size_t len;

    do {
        char* res = fgets(str, sizeof(MAX_STR_SIZE), stdin);
        if (res == NULL) {
            break;
        }
        len = strlen(str);
        next->str = malloc(len + 1);
        check(next->str);
        memcpy(next->str, str, len + 1);
        
        struct Entry* t = malloc(sizeof(struct Entry));
        check(t);
        next->next = t;
        next = t;
        next->next = NULL;
    } while (str[0] != '.');

    next = head;

    while (next != NULL) {
        // printf("AAA\n");
        printf("%s", next->str);
        struct Entry* t = next;
        next = next->next;
        free(t);
        free(t->str);
    }

    return 0;
}

// 0x7fXXXX STACK



// 0x51XXXX HEAP
// 0x5XXXXX CODE
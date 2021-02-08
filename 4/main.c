#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_SIZE 300

struct Entry {
    char* str;
    struct Entry* next;
};

int main() {
    char str[MAX_STR_SIZE + 1];
    struct Entry* head = malloc(sizeof(struct Entry));
    head->next = NULL;

    struct Entry* next = head;
    size_t len;

    do {
        fgets(str, sizeof(MAX_STR_SIZE), stdin);
        len = strlen(str);
        next->str = malloc(len + 1);
        memcpy(next->str, str, len + 1);
        
        struct Entry* t = malloc(sizeof(struct Entry));
        next->next = t;
        next = t;
        next->next = NULL;
    } while (str[0] != '.' || len != 2);

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
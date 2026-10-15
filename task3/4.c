#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024

struct node {
    char *str;
    struct node *next;
};

int main(void)
{
    char buf[MAXLEN];
    struct node *head = NULL, *tail = NULL, *p, *next;
    size_t len;

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (buf[0] == '.')
            break;

        len = strlen(buf);

        p = malloc(sizeof(struct node));
        if (p == NULL) {
            perror("malloc");
            return 1;
        }
        p->str = malloc(len + 1);          
        if (p->str == NULL) {
            perror("malloc");
            return 1;
        }
        strcpy(p->str, buf);
        p->next = NULL;

        if (head == NULL)
            head = p;
        else
            tail->next = p;
        tail = p;
    }

    for (p = head; p != NULL; p = p->next)
        fputs(p->str, stdout);

    for (p = head; p != NULL; p = next) {
        next = p->next;
        free(p->str);
        free(p);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

typedef struct Node {
    char *text;
    struct Node *next;
} Node;

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head->text);
        free(head);
        head = next;
    }
}

int main(void)
{
    char buffer[BUFFER_SIZE];
    Node *head = NULL;
    Node *tail = NULL;
    int status = EXIT_SUCCESS;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        Node *node;
        size_t length;

        if (buffer[0] == '.')
            break;

        length = strlen(buffer);

        /* Проверяем, что строка поместилась в буфер целиком. */
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[--length] = '\0';
        } else {
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                fprintf(stderr, "Строка слишком длинная\n");
                status = EXIT_FAILURE;
                goto cleanup;
            }
        }

        if (ferror(stdin)) {
            fprintf(stderr, "Ошибка чтения\n");
            status = EXIT_FAILURE;
            goto cleanup;
        }

        node = malloc(sizeof(*node));
        if (node == NULL) {
            fprintf(stderr, "Недостаточно памяти\n");
            status = EXIT_FAILURE;
            goto cleanup;
        }

        /* Дополнительный байт нужен для завершающего '\0'. */
        node->text = malloc(length + 1);
        if (node->text == NULL) {
            free(node);
            fprintf(stderr, "Недостаточно памяти\n");
            status = EXIT_FAILURE;
            goto cleanup;
        }

        memcpy(node->text, buffer, length + 1);
        node->next = NULL;

        if (tail == NULL)
            head = node;
        else
            tail->next = node;

        tail = node;
    }

    if (ferror(stdin)) {
        fprintf(stderr, "Ошибка чтения\n");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    for (Node *node = head; node != NULL; node = node->next) {
        if (puts(node->text) == EOF) {
            status = EXIT_FAILURE;
            break;
        }
    }

cleanup:
    free_list(head);
    return status;
}
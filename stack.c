#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct node* create_node(int row, int col)
{
        struct node *new = malloc(sizeof(struct node));
        new->loc.row = row;
        new->loc.col = col;
        new->next = NULL;
        return new;
}

void stack_destroy(struct node *head)
{
        struct node *curr = head->next;
        while (curr != NULL) {
                struct node *prev = curr->next;
                free(curr);
                curr = prev;
        }
        head->next = NULL;
}

void stack_push(struct node *head, int row, int col)
{
        struct node *curr = create_node(row, col);
        curr->next = head->next;
        head->next = curr;
}

struct coord stack_pop(struct node *head)
{
        struct node *curr = head->next;
        struct coord point;
        if (curr != NULL) {
                head->next = curr->next;
                point = curr->loc;
                free(curr);
        }
        return point;
}

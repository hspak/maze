#ifndef STACK_H
#define STACK_H

struct coord {
    long row;
    long col;
};

struct node {
    struct node *next;
    struct coord loc;
};

struct node* create_node(int row, int col);
struct coord stack_pop(struct node *head);
void stack_push(struct node *head, int row, int col);
void stack_destroy(struct node *head);

#endif

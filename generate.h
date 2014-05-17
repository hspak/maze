#ifndef GENERATE_H
#define GENERATE_H
#include "stack.h"

#define W 0
#define S 1
#define E 2
#define N 3

void initialize_cells(long col, uint16_t (*cell)[col], long row);
void generate_maze(long col, uint16_t (*cell)[col], long row, struct node *head);
int find_neighbors(long col, uint16_t (*cell)[col], long row, long sc, long sr, int *dir_good);
void select_dir(long col, uint16_t (*cell)[col], long *sc, long *sr, int *dir_good, struct node *head);

#endif

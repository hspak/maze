#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "generate.h"
#include "stack.h"
#include "visual.h"
#include "maze.h"

int main(int argc, char *argv[])
{
        long row;
        long col;
        char *endptr = NULL;
        if (argc < 3) {
                fprintf(stderr, "usage: maze <row count> <column count>\n");
                return 1;
        }
        row = strtol(argv[1], &endptr, 10);
        if (endptr == NULL || *endptr != (char)0) {
                fprintf(stderr, "error: unknown row count\n");
                return 1;
        }
        col = strtol(argv[2], &endptr, 10);
        if (endptr == NULL || *endptr != (char)0) {
                fprintf(stderr, "error: unknown column count\n");
                return 1;
        }

        // initialize
        uint16_t (*cell)[col] = malloc(sizeof(*cell) * row);
        initialize_cells(col, cell, row);

        // visual init
        long cl = col*2 + 1 + 1; // extra for null char
        long rl = row*2 + 1;
        char (*maze)[cl] = malloc(sizeof(*maze) * rl);
        init_display(cl, maze, rl);

        // create
        struct node *head = create_node(0, 0);
        generate_maze(col, cell, row, head, cl, maze, rl);
        generate_soln(col, cell, head, cl, maze, rl);

#ifdef TESTING
        int ti;
        int tj;
        for (ti = 0; ti <= row-1; ti++) {
                for(tj = 0; tj <= col-1; tj++) {
                        printf("[%d][%d] = %04x\t", tj, ti, cell[ti][tj]);
                }
                printf("\n");
        }
#endif

        // clean
        stack_destroy(head);
        free(head);
        free(maze);
        free(cell);
        return 0;
}

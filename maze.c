#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "generate.h"
#include "stack.h"
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
        col = strtol(argv[1], &endptr, 10);
        if (endptr == NULL || *endptr != (char)0) {
                fprintf(stderr, "error: unknown column count\n");
                return 1;
        }

        // initialize
        uint16_t (*cell)[col] = malloc(sizeof(*cell) * row);
        initialize_cells(col, cell, row);

        // create
        struct node *head = create_node(0, 0);
        generate_maze(col, cell, row, head);

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
        free(cell);
        return 0;
}

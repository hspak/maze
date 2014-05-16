#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stack.h" 
#include "rand.h"
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

        // set random seed
        set_seed();

        // initialize cells
        // 16-bit
        // 0000 0000 0000 0000
        // back soln bord wall
        // WSEN WSEN WSEN WSEN
        uint16_t (*cell)[col] = malloc(sizeof(*cell) * row);
        int rc;
        int cc;
        for (rc = 0; rc < row; rc++) {
                for (cc = 0; cc < col; cc++) {
                        // corners
                        if      (rc == 0 && cc == 0)             cell[rc][cc] = 0x009F;
                        else if (rc == 0 && cc == (col-1))       cell[rc][cc] = 0x003F;
                        else if (rc == (row-1) && cc == 0)       cell[rc][cc] = 0x00CF;
                        else if (rc == (row-1) && cc == (col-1)) cell[rc][cc] = 0x006F;
                        // 2x2 or less
                        else if (rc == (row-1) && rc == 0 && cc == (col-1)) cell[rc][cc] = 0x007F;
                        else if (rc == (row-1) && rc == 0 && cc == 0)       cell[rc][cc] = 0x008F;
                        else if (rc == (row-1) && cc == 0 && cc == (col-1)) cell[rc][cc] = 0x00EF;
                        else if (rc == 0 && cc == 0 && cc == (col-1))       cell[rc][cc] = 0x001F;
                        // borders
                        else if (rc == 0)       cell[rc][cc] = 0x001F;
                        else if (rc == (row-1)) cell[rc][cc] = 0x004F;
                        else if (cc == 0)       cell[rc][cc] = 0x008F;
                        else if (cc == (col-1)) cell[rc][cc] = 0x002F;
                        // inner
                        else                    cell[rc][cc] = 0x000F;
                }
        }

        struct node *head = create_node(0, 0);
        long cell_count = row*col;
        long sc = get_rand(col);
        long sr = get_rand(row);
        printf("AT (%ld, %ld)\n", sc, sr);
        long visited_count = 1;
        int neighbor_count;
        int dir_good[4] = {0,0,0,0};
        while (visited_count < cell_count) {
                // reset
                neighbor_count = 0;
                dir_good[W] = 0;
                dir_good[S] = 0;
                dir_good[E] = 0;
                dir_good[N] = 0;

                // find neighbors with all walls
                if (sc != 0)
                        if ((cell[sr][sc-1] & 0x000F) == 0x000F) {
                                neighbor_count++;
                                dir_good[W] = 1;
                        }
                if (sc != (col-1)) 
                        if ((cell[sr][sc+1] & 0x000F) == 0x000F) {
                                neighbor_count++;
                                dir_good[E] = 1;
                        }
                if (sr != 0)
                        if ((cell[sr-1][sc] & 0x000F) == 0x000F) {
                                neighbor_count++;
                                dir_good[N] = 1;
                        }
                if (sr != (col-1)) 
                        if ((cell[sr+1][sc] & 0x000F) == 0x000F) {
                                neighbor_count++;
                                dir_good[S] = 1;
                        }

                // pick a direction to go
                if (neighbor_count > 0) {
                        // should be random, but doesn't matter (I think)
                        if (dir_good[N]) {
                                cell[sr][sc] = cell[sr][sc] & 0xFFFE;
                                sr--;
                                printf("AT (%ld, %ld)\n", sc, sr);
                                stack_push(head, sr, sc);
                                cell[sr][sc] = cell[sr][sc];
                                cell[sr][sc] = cell[sr][sc] & 0xFFFB;
                        } else if (dir_good[E]) {
                                cell[sr][sc] = cell[sr][sc] & 0xFFFD;
                                sc++;
                                printf("AT (%ld, %ld)\n", sc, sr);
                                stack_push(head, sr, sc);
                                cell[sr][sc] = cell[sr][sc];
                                cell[sr][sc] = cell[sr][sc] & 0xFFF7;
                        } else if (dir_good[S]) {
                                cell[sr][sc] = cell[sr][sc] & 0xFFFB;
                                sr++;
                                printf("AT (%ld, %ld)\n", sc, sr);
                                stack_push(head, sr, sc);
                                cell[sr][sc] = cell[sr][sc];
                                cell[sr][sc] = cell[sr][sc] & 0xFFFE;
                        } else if (dir_good[W]) {
                                cell[sr][sc] = cell[sr][sc] & 0xFFF7;
                                sc--;
                                printf("AT (%ld, %ld)\n", sc, sr);
                                stack_push(head, sr, sc);
                                cell[sr][sc] = cell[sr][sc];
                                cell[sr][sc] = cell[sr][sc] & 0xFFFD;
                        }
                        visited_count++;
                } else {
                        // backtrack
                        struct coord loc = stack_pop(head); 
                        if (loc.col > sc) cell[sr][sc] = cell[sr][sc] | 0x2000; // east
                        if (loc.col < sc) cell[sr][sc] = cell[sr][sc] | 0x8000; // west
                        if (loc.row > sr) cell[sr][sc] = cell[sr][sc] | 0x4000; // south
                        if (loc.row < sr) cell[sr][sc] = cell[sr][sc] | 0x1000; // north

                        // new loc
                        sc = loc.col;
                        sr = loc.row;
                        printf("BA (%ld, %ld)\n", sc, sr);
                }
        }

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

        stack_destroy(head);
        free(head);
        free(cell);
        return 0;
}

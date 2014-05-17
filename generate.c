#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "stack.h"
#include "rand.h"
#include "visual.h"
#include "generate.h"

// 16-bit
// 0000 0000 0000 0000
// back soln bord wall
// WSEN WSEN WSEN WSEN
void initialize_cells(long col, uint16_t (*cell)[col], long row)
{
        for (int rc = 0; rc < row; rc++) {
                for (int cc = 0; cc < col; cc++) {
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
}

void generate_maze(long col, uint16_t (*cell)[col], long row, struct node *head)
{
        set_seed();

        long cl = col*2 + 1 + 1; // extra for null char
        long rl = row*2 + 1;
        char (*maze)[cl] = malloc(sizeof(*maze) * rl);
        init_display(cl, maze, rl);

        long cell_count = row*col;
        long visited_count = 1;
        long sc = get_rand(col);
        /* long sr = get_rand(row); */
        long sr = row-1;
        printf("AT (%ld, %ld)\n", sc, sr);
        int *dir_good = malloc(sizeof(int) * 4);
        update_maze(cl, maze, rl, sr, sc, PATH);
        while (visited_count < cell_count) {
                int neighbor_count = find_neighbors(col, cell, row, sc, sr, dir_good);
                if (neighbor_count > 0) {
                        // pick a direction to go
                        select_dir(col, cell, &sc, &sr, dir_good, head);
                        update_maze(cl, maze, rl, sr, sc, PATH);
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
                        update_maze(cl, maze, rl, sr, sc, BACK);
                }
                sleep(1);
        }
        free(dir_good);
}

void select_dir(long col, uint16_t (*cell)[col], long *sc, long *sr, int *dir_good, struct node *head)
{
        // (maze too simple without random)
        if (dir_good[N]) {
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFE;
                (*sr)--;
                printf("AT (%ld, %ld)\n", *sc, *sr);
                stack_push(head, *sr, *sc);
                cell[*sr][*sc] = cell[*sr][*sc];
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFB;
        } else if (dir_good[E]) {
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFD;
                (*sc)++;
                printf("AT (%ld, %ld)\n", *sc, *sr);
                stack_push(head, *sr, *sc);
                cell[*sr][*sc] = cell[*sr][*sc];
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFF7;
        } else if (dir_good[S]) {
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFB;
                (*sr)++;
                printf("AT (%ld, %ld)\n", *sc, *sr);
                stack_push(head, *sr, *sc);
                cell[*sr][*sc] = cell[*sr][*sc];
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFE;
        } else if (dir_good[W]) {
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFF7;
                (*sc)--;
                printf("AT (%ld, %ld)\n", *sc, *sr);
                stack_push(head, *sr, *sc);
                cell[*sr][*sc] = cell[*sr][*sc];
                cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFD;
        }
}

int find_neighbors(long col, uint16_t (*cell)[col], long row, long sc, long sr, int *dir_good)
{
        int neighbor_count = 0;
        for(int i = 0; i < 4; i ++) dir_good[i] = 0;

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
        if (sr != (row-1))
                if ((cell[sr+1][sc] & 0x000F) == 0x000F) {
                        neighbor_count++;
                        dir_good[S] = 1;
                }
        return neighbor_count;
}

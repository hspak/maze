#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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

void generate_maze(long col, uint16_t (*cell)[col], long row, struct node *head
                  ,long cl, char (*maze)[cl], long rl)
{
        set_seed();
        long cell_count = row*col;
        long visited_count = 1;

        // selected col/row
        long sc = get_rand(col);
        long sr = get_rand(row);

        // keep 1 before for visual (knock down wall)
        long sc_old = sc;
        long sr_old = sr;

        /* printf("AT (%ld, %ld)\n", sc, sr); */
        int *dir_good = malloc(sizeof(int) * 4);
        update_maze(cl, maze, rl, sr, sc, sr_old, sc_old, START);
        while (visited_count < cell_count) {
                int neighbor_count = find_neighbors(col, cell, row, sc, sr, dir_good);
                if (neighbor_count > 0) {
                        // pick a direction to go
                        sc_old = sc;
                        sr_old = sr;
                        select_dir(col, cell, &sc, &sr, dir_good, head);
                        update_maze(cl, maze, rl, sr, sc, sr_old, sc_old, PATH);
                        visited_count++;
                } else {
                        // backtrack
                        struct coord loc = stack_pop(head);
                        if (loc.col > sc) cell[sr][sc] = cell[sr][sc] | 0x2000; // east
                        if (loc.col < sc) cell[sr][sc] = cell[sr][sc] | 0x8000; // west
                        if (loc.row > sr) cell[sr][sc] = cell[sr][sc] | 0x4000; // south
                        if (loc.row < sr) cell[sr][sc] = cell[sr][sc] | 0x1000; // north

                        // new loc
                        update_maze(cl, maze, rl, loc.row, loc.col, sr, sc, BACK);
                        sc = loc.col;
                        sr = loc.row;
                        /* printf("BA (%ld, %ld)\n", sc, sr); */
                }
        }
        update_maze(cl, maze, rl, sr, sc, sr_old, sc_old, END);
        free(dir_good);
}

void generate_soln(long col, uint16_t (*cell)[col], struct node *head
                  ,long cl, char (*maze)[cl], long rl)
{
        struct coord prev = stack_pop(head);
        struct coord curr;
        update_maze(cl, maze, rl, prev.row, prev.col, prev.row, prev.col, SOLN);
        while (head->next) {
                curr = stack_pop(head);
                if (prev.row != curr.row && prev.col != curr.col) {
                        fprintf(stderr, "error: something broke finding sol'n (%ld, %ld) -> (%ld, %ld)\n",
                                prev.col, prev.row, curr.col, curr.row);
                } if (prev.row > curr.row) { // south
                        cell[curr.row][curr.col] = cell[curr.row][curr.col] | 0x0400;
                } else if (prev.row < curr.row) { // north
                        cell[curr.row][curr.col] = cell[curr.row][curr.col] | 0x0100;
                } else if (prev.col > curr.col) { // west
                        cell[curr.row][curr.col] = cell[curr.row][curr.col] | 0x0800;
                } else if (prev.col < curr.col) { // east
                        cell[curr.row][curr.col] = cell[curr.row][curr.col] | 0x0200;
                } else {
                        fprintf(stderr, "error: something broke finding sol'n (%ld, %ld) -> (%ld, %ld)\n",
                                prev.col, prev.row, curr.col, curr.row);
                }
                /* printf("SOL'N (%ld, %ld)\n", curr.row, curr.col); */
                /* printf("(%ld, %ld)->(%ld, %ld)\n", prev.row, prev.col, curr.row, curr.col); */
                update_maze(cl, maze, rl, prev.row, prev.col, curr.row, curr.col, SOLN);
                prev = curr;
        }
}

void select_dir(long col, uint16_t (*cell)[col], long *sc, long *sr, int *dir_good, struct node *head)
{
        int rand = get_rand(4);
        while (!dir_good[rand]) rand = get_rand(4);
        switch(rand)
        {
                case(N):
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFE;
                        stack_push(head, *sr, *sc);
                        (*sr)--;
                        /* printf("AT (%ld, %ld)\n", *sc, *sr); */
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFB;
                        break;
                case(E):
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFD;
                        stack_push(head, *sr, *sc);
                        (*sc)++;
                        /* printf("AT (%ld, %ld)\n", *sc, *sr); */
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFF7;
                        break;
                case(S):
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFB;
                        stack_push(head, *sr, *sc);
                        (*sr)++;
                        /* printf("AT (%ld, %ld)\n", *sc, *sr); */
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFE;
                        break;
                case(W):
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFF7;
                        stack_push(head, *sr, *sc);
                        (*sc)--;
                        /* printf("AT (%ld, %ld)\n", *sc, *sr); */
                        cell[*sr][*sc] = cell[*sr][*sc] & 0xFFFD;
                        break;
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

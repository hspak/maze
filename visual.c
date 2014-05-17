#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "visual.h"

void init_display(long cl, char (*maze)[cl], long rl)
{
        int altc = 0;
        int altr = 0;
        for(long rc = 0; rc < rl; rc++) {
                for(long cc = 0; cc < (cl-1); cc++) {
                        if (altr) {
                                if (altc) maze[rc][cc] = '|';
                                else maze[rc][cc] = ' ';
                        } else {
                                if (altc) maze[rc][cc] = '-';
                                else maze[rc][cc] = '+';
                        }
                        altc = ~altc;
                        printf("%c", maze[rc][cc]);
                }
                altr = ~altr;
                maze[rc][cl-1] = '\0';
                printf("\n");
        }
}

void update_maze(long cl, char (*maze)[cl], long rl, long sr, long sc, long sr_old, long sc_old, int type)
{
        int srl = sr*2 + 1;
        int scl = sc*2 + 1;
        char symbol[5] = {'o', 'x', '*', 'S', 'E'};

        if (type == BACK) {
                maze[sr_old*2+1][sc_old*2+1] = 'x';
        }

        if (sr > sr_old)
                maze[srl-1][scl] = symbol[type];
        else if (sr < sr_old)
                maze[srl+1][scl] = symbol[type];
        else if (sc > sc_old)
                maze[srl][scl-1] = symbol[type];
        else if (sc < sc_old)
                maze[srl][scl+1] = symbol[type];
        else if (sc != sc_old && sr != sr_old)
                fprintf(stderr, "error: something broke traversing (%ld, %ld) -> (%ld, %ld)\n",
                        sc_old, sr_old, sc, sr);

        if (maze[srl][scl] != 'E' && maze[srl][scl] != 'S')
                maze[srl][scl] = symbol[type];
        display_maze(cl, maze, rl);
        usleep(100000);
}

void display_maze(long cl, char (*maze)[cl], long rl)
{
        for (long rc = 0; rc < rl; rc++) {
                for (long cc = 0; cc < cl; cc++) {
                        if (maze[rc][cc] == '|' || maze[rc][cc] == '+' || maze[rc][cc] == '-')
                                printf("%s%c%s", BOLDBLACK, maze[rc][cc], NC);
                        else if (maze[rc][cc] == '*' || maze[rc][cc] == 'E' || maze[rc][cc] == 'S')
                                printf("%s%c%s", GREEN, maze[rc][cc], NC);
                        else if (maze[rc][cc] == 'x')
                                printf("%s%c%s", RED, maze[rc][cc], NC);
                        else
                                printf("%s%c%s", BLUE, maze[rc][cc], NC);
                }
                printf("\n");
        }
        printf("\n\n");
}

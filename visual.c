#include <stdio.h>
#include <stdlib.h>
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
                }
                altr = ~altr;
                maze[rc][cl] = '\0';
        }
}

void update_maze(long cl, char (*maze)[cl], long rl, long sr, long sc, int type)
{
        char symbol[3] = {'o', 'x', '0'};
        maze[sr*2+1][sc*2+1] = symbol[type];
        display_maze(cl, maze, rl);
}

void display_maze(long cl, char (*maze)[cl], long rl)
{
        for(long rc = 0; rc < rl; rc++) {
                printf("%s\n", maze[rc]);
        }
}

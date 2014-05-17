#ifndef VISUAL_H
#define VISUAL_H

#define PATH 0
#define BACK 1
#define SOLN 2

void init_display(long cl, char (*maze)[cl], long rl);
void display_maze(long cl, char (*maze)[cl], long rl);
void update_maze(long cl, char (*maze)[cl], long rl, long sr, long sc, int type);

#endif

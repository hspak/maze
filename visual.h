#ifndef VISUAL_H
#define VISUAL_H

#define NC                    "\033[0m"
#define BLACK                 "\033[0;30m"
#define RED                   "\033[0;31m"
#define GREEN                 "\033[0;32m"
#define YELLOW                "\033[0;33m"
#define BLUE                  "\033[0;34m"
#define MAGENTA               "\033[0;35m"
#define CYAN                  "\033[0;36m"
#define WHITE                 "\033[0;37m"
#define BOLDBLACK             "\033[1;30m"
#define BOLDRED               "\033[1;31m"
#define BOLDGREEN             "\033[1;32m"
#define BOLDYELLOW            "\033[1;33m"
#define BOLDBLUE              "\033[1;34m"
#define BOLDMAGENTA           "\033[1;35m"
#define BOLDCYAN              "\033[1;36m"
#define BOLDWHITE             "\033[1;37m"

#define PATH 0
#define BACK 1
#define SOLN 2
#define START 3
#define END 4

void init_display(long cl, char (*maze)[cl], long rl);
void display_maze(long cl, char (*maze)[cl], long rl);
void update_maze(long cl, char (*maze)[cl], long rl, long sr, long sc, long sr_old, long sc_old, int type);

#endif

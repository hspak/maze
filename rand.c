#include <stdlib.h>
#include <time.h>

void set_seed()
{
        srand(time(NULL));
}

int get_rand(int ub)
{
        return rand() % ub;
}

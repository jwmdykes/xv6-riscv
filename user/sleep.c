#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Usage: %s [time]", argv[0]);
    }

    return sleep(atoi(argv[1]));
}
#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("Format: %s [system call number] [program to run] [[args]]", argv[0]);
        return 1;
    }

    trace(atoi(argv[1]));
    exec(argv[2], &argv[2]);
    printf("exec error\n");

    int pid = fork();
    if (pid == 0)
    {
        trace(atoi(argv[1]));
        exec(argv[2], &argv[2]);
        printf("exec error\n");
    }
    else
    {
        wait(0);
        return 0;
    }
}
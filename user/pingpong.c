#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid == 0)
    {
        char byte;
        read(p[0], &byte, 1);

        printf("%d: received ping byte: %d\n", getpid(), byte);
        write(p[1], &byte, 1);
        return 0;
    }

    char byte = 0x2a;
    write(p[1], &byte, 1);
    wait(0);
    read(p[0], &byte, 1);
    printf("%d: received pong byte: %d\n", getpid(), byte);

    close(p[0]);
    close(p[1]);
    return 0;
}

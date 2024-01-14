#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// parent process
int parent(int p[])
{
    int pid = getpid();

    write(p[1], "ping", 4);

    wait(0);

    char msg[4];
    read(p[0], msg, 4);

    printf("%d: read %s\n", pid, msg);

    return 0;
}

int child(int p[])
{
    int pid = getpid();

    write(p[1], "pong", 4);

    char msg[4];
    read(p[0], msg, 4);

    printf("%d: read %s\n", pid, msg);

    return 0;
}

int main(void)
{
    int p[2];
    pipe(p);

    int child_pid = fork();
    if (child_pid == 0)
    {
        // we are the child
        int err = child(p);
        exit(err);
    }
    else
    {
        // we are the parent
        int err = parent(p);
        write(1, "\n", 1);
        exit(err);
    }
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void right_process(int p[2])
{
    int prime;
    int n;
    close(p[1]); // close the write end

    if (read(p[0], &prime, sizeof(int)) != sizeof(int))
    {
        fprintf(2, "Error while reading.\n");
        exit(1);
    }

    printf("prime %d\n", prime);

    // if there's no more stuff, don't make a new process
    if (read(p[0], &n, sizeof(int)) != sizeof(int))
    {
        exit(0);
    }

    // create a new process
    int new_pipe[2];
    pipe(new_pipe);

    if (fork() == 0)
    {
        right_process(new_pipe);
    }
    else
    {
        close(new_pipe[0]);
        do
        {
            if (n % prime != 0)
                write(new_pipe[1], &n, sizeof(int));
        } while (read(p[0], &n, sizeof(int)) == sizeof(int));
        close(new_pipe[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        right_process(p);
        exit(0);
    }

    // parent
    close(p[0]);
    int i;
    for (i = 2; i <= 35; i++)
    {
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
    exit(0);
    return 0;
}
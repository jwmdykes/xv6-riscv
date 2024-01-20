#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void do_exec(char *args[])
{
    if (fork() == 0)
    {
        char **p = args;
        while (*p != 0)
        {
            p++;
        }

        exec(args[0], args);
    }
    wait(0);
}

void xargs(int argc, char *argv[])
{
    char *all_args[MAXARG];

    int num_args;
    for (num_args = 0; num_args < argc - 1; num_args++)
    {
        all_args[num_args] = argv[num_args + 1];
    }

    char *string = (char *)malloc(256 * sizeof(char));
    all_args[num_args] = string;
    while (read(0, string, sizeof(char)) == sizeof(char) && num_args < MAXARG - 1)
    {
        if (*string == ' ')
        {
            *string = 0; // null terminate string
            string = (char *)malloc(256 * sizeof(char));
            num_args++;
            all_args[num_args] = string;
        }
        else if (*string == '\n')
        {
            *string = 0;
            all_args[num_args + 1] = 0;
            do_exec(all_args);

            // free strings
            int i;
            for (i = argc - 1; i <= num_args; i++)
            {
                free(all_args[i]);
            }

            // create new string
            string = (char *)malloc(256 * sizeof(char));
            num_args = argc - 1;
            all_args[num_args] = string;
        }
        else
        {
            string++;
        }
    }
}

int main(int argc, char *argv[])
{
    xargs(argc, argv);
    return 0;
}
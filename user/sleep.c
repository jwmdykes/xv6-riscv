#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char help_message[] = "usage: sleep [time]\n";
        write(1, help_message, strlen(help_message));
        exit(-1);
    }

    int time = atoi(argv[1]);
    sleep(time);

    exit(0);
}
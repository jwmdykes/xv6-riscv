#include <kernel/types.h>
#include <kernel/stat.h>
#include "kernel/fs.h"
#include <user/user.h>

void print_st(struct stat st)
{
    printf("dev: %d\n", st.dev);
    printf("ino: %d\n", st.ino);
    printf("nlink: %d\n", st.nlink);
    printf("size: %d\n", st.size);
    printf("size: %d\n", st.type);
}

int find(char file_name[], char path[])
{
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        // fprintf(2, "ls: cannot open %s\n", path);
        return -1;
    }

    if (fstat(fd, &st) < 0)
    {
        // fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return -1;
    }

    if (st.type != T_DIR)
    {
        // fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return -1;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        if (strcmp(de.name, file_name) == 0)
            printf("%s/%s\n", path, de.name);

        char new_path[512];
        if (strlen(path) + 1 + strlen(de.name) + 1 > sizeof new_path)
        {
            fprintf(2, "find: path too long\n");
            continue;
        }
        strcpy(new_path, path);
        strcpy(new_path + strlen(path), "/");
        strcpy(new_path + strlen(path) + 1, de.name);
        find(file_name, new_path); // Recursive call for subdirectories
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: %s [filename]\n", argv[0]);
        return 0;
    }

    return find(argv[1], ".");
}
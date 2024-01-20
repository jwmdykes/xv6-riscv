#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char path[], char name[])
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        goto cleanup;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        goto cleanup;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("ls: path too long\n");
        goto cleanup;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        if (strcmp(de.name, ".") == 0)
        {
            continue;
        }
        else if (strcmp(de.name, "..") == 0)
        {
            continue;
        }
        else if (strcmp(de.name, name) == 0)
        {
            printf("%s/%s\n", path, de.name);
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_DIR)
        {
            find(buf, name);
        }
    }

cleanup:
    close(fd);
    return;
}

int main(int argc, char *argv[])
{
    struct stat st;

    if (argc <= 2)
    {
        printf("Usage: %s [[path]] [name]\n", argv[0]);
        return 1;
    }

    stat(argv[1], &st);
    if (st.type != T_DIR)
    {
        fprintf(2, "Not a directory\n");
    }

    find(argv[1], argv[2]);

    return 0;
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *fileName)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // printf("type of input: %d\n", st.type);

    if (st.type == T_DIR) {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            exit(1);
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0|| strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if (strcmp(de.name, fileName) == 0)
            {
                printf("%s/%s\n", path, de.name);
            }
            if (st.type == T_DIR)
            {
                find(buf, fileName);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    // Ensure correct number of arguments
    if (argc != 3)
    {
        printf("Usage: find <directory> <filename>\n");
        exit(1);
    }

    char *dirName = argv[1];
    char *fileName = argv[2];

    find(dirName, fileName);
    exit(0);
}

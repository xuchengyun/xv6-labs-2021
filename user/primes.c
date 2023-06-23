#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int read_fd)
{
    int first; // first is the prime number
    int n;
    int pid;
    // fd[0] represents the read end of the pipe
    // fd[1] represents the write end of the pipe
    int fd[2];
    // printf("pid: %d\n", getpid());

    if (read(read_fd, &first, sizeof(first)) <= 0)
    {
        close(read_fd);
        exit(0);
    }

    printf("prime %d\n", first);

    if (pipe(fd) < 0)
    {
        printf("pipe failed\n");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        fprintf(2, "Failed to fork\n");
        exit(1);
    }
    if (pid == 0)
    {
        close(fd[1]); // close write end in child process
        prime(fd[0]);
    }
    else
    {
        while (1)
        {
            // read value from pipe to n
            if (read(read_fd, &n, sizeof(n)) <= 0)
            { // if write end closed, read will return 0
                close(read_fd);
                break;
            }
            if (n % first != 0) {
                write(fd[1], &n, sizeof(n));
            }
        }
        close(fd[1]); // close write end
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    int i;
    int fd[2];
    int pid;

    if (pipe(fd) < 0)
    {
        printf("pipe failed\n");
        exit(1);
    }

    pid = fork();
    // error handling
    if (pid < -1)
    {
        fprintf(2, "Failed to fork\n");
        exit(1);
    }
    if (pid == 0)
    {
        // Child process
        close(fd[1]);
        prime(fd[0]);
    }
    else
    {
        // parent process
        // Generate integer from 2 to 35
        for (i = 2; i <= 35; i++)
        {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]); // close write end
        wait(0);
    }
    exit(0);
}
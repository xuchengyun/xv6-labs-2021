#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent_to_child[2], child_to_parent[2];
    int pid;

    // Create the two pipes
    if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0)
    {
        fprintf(2, "Failed to create pipe\n");
        exit(1);
    }

    // Fork a child process
    pid = fork();
    if (pid < -1)
    {
        fprintf(2, "Failed to fork\n");
        exit(1);
    }

    if (pid == 0)
    { // Child process
        // close(parent_to_child[1]);  // Close write end of parent_to_child pipe
        // close(child_to_parent[0]);  // Close read end of child_to_parent pipe

        // Read a byte from the parent process
        char buf[512];
        if (read(parent_to_child[0], buf, sizeof buf) < 0)
        {
            fprintf(2, "Failed to read from pipe\n");
            exit(1);
        }

        printf("%d: received ping\n", getpid());

        // Write the byte back to the parent process
        if (write(child_to_parent[1], buf, sizeof buf) < 0)
        {
            fprintf(2, "Failed to write to pipe\n");
            exit(1);
        }

        exit(0);
    }
    else
    { // Parent process
        // close(parent_to_child[0]);  // Close read end of parent_to_child pipe
        // close(child_to_parent[1]);  // Close write end of child_to_parent pipe

        // Write a byte to the child process
        char buf[512];
        if (write(parent_to_child[1], buf, sizeof buf) < -1)
        {
            fprintf(2, "Failed to write to pipe\n");
            exit(1);
        }

        // Read the byte back from the child process
        if (read(child_to_parent[0], buf, sizeof buf) == -1)
        {
            fprintf(2, "Failed to read from pipe\n");
            exit(1);
        }

        printf("%d: received pong\n", getpid());
    }

    exit(0);
}

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("xargs : minum amount of args is 2 !\n");
        exit(1);
    }
    if (argc - 1 >= MAXARG)
    {
        printf("xargs : maxium amount of args is %d !\n", MAXARG);
        exit(1);
    }

    char *x_args[MAXARG];

    // 1.copy args in argv[] to the head of x_args[]
    for (int i = 1; i < argc; i++)
    {
        x_args[i - 1] = argv[i];
    }

    // 2.copy the output of previous command to the tail of x_args[]
    // char buf[512];
    char *buf = (char *)malloc(512);
    int i = 0;
    int index = argc - 1; // index of x_args[]
    while (read(0, &buf[i], sizeof(char)))
    {
        char c = buf[i]; // Copy the value to c
        if (c == ' ' || c == '\n')
        {
            buf[i] = 0; // end
            x_args[index] = buf;
            char *temp_buf = (char *)malloc(512);
            free(buf);
            buf = temp_buf;
            index++;
            i = 0;
            if (c == '\n')
            {
                x_args[index] = 0;
                index = argc - 1; // relocate the index
                int pid = fork();
                if (pid < 0)
                {
                    printf("fork error !\n");
                    exit(1);
                }
                else if (pid == 0)
                {
                    exec(x_args[0], x_args);
                    fprintf(2, "exec failed\n");
                    exit(1);
                }
                else
                {
                    wait(0);
                }
            }
        }
        else
        {
            i++;
        }
    }
    free(buf); // free the remaining buf before exit
    exit(0);
}

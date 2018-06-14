#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <stdlib.h>
#define LEN 256


char command1[LEN];
void split(char buf[], char *array[])
{
    int i = 0;
    char *p = strtok(buf, " ");

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
}

int execute(char *array[])
{
    if (strcmp(array[0], "echo") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            int i;
            for (i = 1; i < 6; i++)
            {
                if (array[i] == '\0')
                    break;
                if (i > 0)
                {
                    write(STDOUT_FILENO, " ", 1);
                }
                write(STDOUT_FILENO, array[i], sizeof(array[i]));
            };
            write(STDOUT_FILENO, "\n", 1);
        }
        else
        {
            int status;
            wait(&status);
        }
    }
    else if (strcmp(array[0], "ls") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            if (array[1][0] == '\0')
            {
                execlp("/bin/ls", "ls", "./", NULL, NULL, NULL);
            }
            else
            {
                execlp("/bin/ls", "ls", array[1], NULL, NULL, NULL);
            }
        }
        else
        {
            int status;
            wait(&status);
        }
    }
    else if (strcmp(array[0], "exit") == 0)
    {
        return -1;
    }
    else if (strcmp(array[0], "cd") == 0)
    {
        chdir(array[1]);
    }
    else if (strcmp(array[0], "pwd") == 0)
    {

        system("pwd");
    }
    else if (strcmp(array[0], "cat") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            execl("/bin/cat", "cat", array[1], NULL);
        }
        else
        {
            int status;
            wait(&status);
        }
    }
    return 1;
}

int main()
{
    while (1)
    {
        char command[LEN];
        char *array[6] = {
            "\0",
            "\0",
            "\0",
            "\0",
            "\0",
            "\0",
        };
        int i;
        printf("%s", ">>");
        gets(command);
        strcpy(command1, command);
        split(command, array);
        execute(array);

    }

    getchar();
}

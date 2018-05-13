#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char **split(char str[])
{
    // printf("%s\n", str);
    // char str[] = "echo qwe sdfsa sdaf";
    char **res = NULL;
    char *p = strtok(str, " ");
    int n_spaces = 0, i;

    /* split string and append tokens to 'res' */

    while (p)
    {
        res = realloc(res, sizeof(char *) * ++n_spaces);

        if (res == NULL)
            exit(-1); /* memory allocation failed */

        res[n_spaces - 1] = p;

        p = strtok(NULL, " ");
    }

    /* realloc one extra element for the last NULL */

    res = realloc(res, sizeof(char *) * (n_spaces + 1));
    res[n_spaces] = 0;

    // for (i = 0; i < (n_spaces + 1); ++i)
    //     printf("res[%d] = %s\n", i, res[i]);

    /* free the memory allocated */

    free(res);
    return res;
}

int main(int argc, char *argv[])
{   
    // int error;
    pid_t pid;
    // char pch = strtok(str, " ");
    pid = fork();
    if (pid == 0)
    {   
        char str[] = "echo ad b c d";
        char **str_temp = split(str);
        int error;
        error = execl("/bin/echo", str_temp[0], str_temp[1], str_temp[2], NULL);
        if (error < 0)
            perror("execl");
    }
    else
    {
        char str[] = "ls /";
        char **str_temp = split(str);
        int error;
        error = execl("/bin/ls", str_temp[0], str_temp[1], NULL);
        if (error < 0)
            perror("execl");
    }


    printf("--------------------------------------------------\n");
    char str[] = "echo HELLO WORLD nmm";
    char **str_array = split(str);
    return 0;
}

// void mysys(char str[])
// {

//     int error;
//     pid_t pid;
//     char pch = strtok(str, " ");
//     pid = fork();
//     if (pid == 0)
//     {
//         error = execlp("/bin/echo", "echo", "a", "b", "c", NULL);
//         if (error < 0)
//             perror("execl");
//     }
//     else
//     {

//     }
// }
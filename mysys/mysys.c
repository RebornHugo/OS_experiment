#include <stdio.h>
#include <unistd.h>
#include <string.h>

void mysys(char str[])
{

    int error;
    pid_t pid;
    char 
    pch = strtok(str," ");
    pid = fork();
    if (pid == 0) {
       error =  execlp("/bin/echo", "echo", "a", "b", "c", NULL); 
       if (error < 0)
           perror("execl");
    }
}





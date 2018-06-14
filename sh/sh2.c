#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define LEN 256
#define N_ARRAY 10

char command_fix[LEN];

void split(char buf[], char *array[]) {
    int i = 0;
    char *p = strtok(buf, " ");

    while (p != NULL) {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
    for (; i < N_ARRAY; i++) {
        array[i] = NULL;
    }
}

int execute(char *array[]) {
    if (strcmp(array[0], "echo") == 0) {
        int pid = fork();
        if (pid == 0) {
            execl("/bin/sh", "sh", "-c", command_fix, NULL);
        } else {
            int status;
            wait(&status);
        }
    } else if (strcmp(array[0], "ls") == 0) {
        int pid = fork();
        if (pid == 0) {
            if (array[1] == NULL) {
                execlp("/bin/ls", "ls", "./", NULL, NULL, NULL);
            } else {
                execlp("/bin/ls", "ls", array[1], NULL, NULL, NULL);
            }
        } else {
            int status;
            wait(&status);
        }
    } else if (strcmp(array[0], "exit") == 0) {
        return -1;
    } else if (strcmp(array[0], "cd") == 0) {
        chdir(array[1]);
    } else if (strcmp(array[0], "pwd") == 0) {
        int pid = fork();
        if (pid == 0) {
            printf("test pwd!!!\n");
            system("pwd");
        } else {
            int status;
            wait(&status);
        }
    } else if (strcmp(array[0], "cat") == 0) {
        int pid = fork();
        if (pid == 0) {
            execl("/bin/cat", "cat", array[1], NULL);
        } else {
            int status;
            wait(&status);
        }
    }
    return 1;
}

int main() {
    while (1) {
        char command[LEN];
        char *array[N_ARRAY];

        printf("%s", ">>");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0'; // ignore newline!!!
        strcpy(command_fix, command);
        split(command, array);
        /*for (int j = 0; j < 10; j++) {
            printf("***%s***%d \n", array[j], array[j] == NULL);
        }*/
        if (-1 == execute(array))
            return 0;
    }
}

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// int open(const char *pathname, int flags);
int main(int argc, char *argv[])
{
    char *path = argv[1];
    int file;
    file = open(path, O_RDONLY);
    if (file == -1)
    {
        printf("unable to open %s", argv[1]);
    }
    char *buf[100000];
    int length;
    length = read(file, buf, 100000);
    if (length != -1)
    {
        printf("%s", buf);
        // printf("%d", length);
    }
    else
    {
        printf("unable to open %s", argv[1]);
    }

    close(file);
    printf("\n");
    return 0;
}

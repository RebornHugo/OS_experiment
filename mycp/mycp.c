#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


//int open(const char *pathname, int flags);
 int main(int argc, char *argv[]){
     char *path = argv[1];
     int file;
      file = open(path, O_RDONLY);
      if (file == -1) {
          printf("unable to open %s", argv[1]);
      }
      char *buf[100000];
      int length;
      length = read(file, buf, 100000);
      if (length != -1){
          // printf("%s", buf);
          int file_to_write;
          file_to_write =  open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
          if (write(file_to_write, buf, length) != -1) {
              printf("finish write file!!!");
          }
          else {
              printf("unable to write %s", argv[2]);
          }

      }
      else {
          printf("unable to open %s", argv[1]);
      }
      printf("\n");
      return 0;
 }


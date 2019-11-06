#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

int testGetOpt(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "abf:")) != -1) {
    switch (c) {
      case 'a':printf("option a\n");
        break;
      case 'b':printf("option b\n");
        break;
      case 'f':printf("file %s", optarg);
        break;
      case '?':errx(1, "Unknown option %c", optopt);
      default:printf("default\n");
    }
  }
  return 0;
}

int openFile(const char *path, int oflag, int mflag) {
  int fd = open(path, oflag, mflag);
  if (fd == -1) {
    errx(1, "Error: unable to open file %s", path);
  }
  return fd;
}

void printFile(char *path) {
  int fd = openFile(path, O_RDONLY, NULL);
  /*char buffer[128];
  ssize_t bytes_read;
  bytes_read = read (fd, buffer, sizeof (buffer));
  printf ("%s\n", buffer);*/
  char t = 0;
  while (read(fd, &t, 1)) {
    printf("%c", t);
    if (t == '\n') {
      poll(NULL, 0, 10);
    }
  }
  close(fd);
}

void copyFile(char *path1, char *path2) {
  int fd1 = openFile(path1, O_RDONLY, NULL);
  int fd2 = openFile(path2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  char t = 0;
  while (read(fd1, &t, 1)) {
    write(fd2, &t, 1);
  }
  close(fd1);
  close(fd2);
}

void printReverse(char *path) {
  int fd = openFile(path, O_RDONLY, NULL);
  while (lseek(fd, -2, SEEK_END) > 0) {
    char t = 0;
    read(fd, &t, 1);
    printf("%c", t);
  }
  printf("%d\n", errno);
}

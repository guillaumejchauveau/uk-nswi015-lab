#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void first() {
  int fd[2];
  if (pipe(fd) < 0) {
    perror("Pipe creation failed");
    exit(-1);
  }

  switch (fork()) {
    case -1:
      perror("Fork failed");
      exit(-1);
    case 0:
      close(fd[1]);
      char c;
      if (read(fd[0], &c, 1) < 0) {
        perror("Read failed");
        exit(-1);
      }
      close(fd[0]);
      exit(c);
    default:
      close(fd[0]);
      if (write(fd[1], "a", 1) < 0) {
        perror("Write failed");
        exit(-1);
      }
      close(fd[1]);

      int commandStatus;
      while (wait(&commandStatus) != -1) {
        fprintf(stderr, "Child exited with status '%c'\n", WEXITSTATUS(commandStatus));
      }

      if (errno != ECHILD) {
        perror("Wait failed");
        exit(-1);
      }
  }
}

void second() {
  int fd[2];
  if (pipe(fd) < 0) {
    perror("Pipe creation failed");
    exit(-1);
  }

  switch (fork()) {
    case -1:
      perror("Fork failed");
      exit(-1);
    case 0:
      close(fd[1]);
      char c;
      if (read(fd[0], &c, 1) < 0) {
        perror("Read failed");
        exit(-1);
      }
      int filed2 = open("a.txt", O_RDONLY);
      if (filed2 == -1) {
        perror("Error: unable to open file");
        exit(-1);
      }
      char *region2 = mmap(NULL, 100, PROT_READ, MAP_SHARED, filed2, 0);
      if (region2 < 0) {
        perror("Mmap failed");
        exit(-1);
      }

      int length = *region2;
      for (int i = 1; i <= length; i++) {
        fprintf(stderr, "%c", *(region2 + i));
      }
      fprintf(stderr, "\n");

      if (munmap(region2, 100) < 0) {
        perror("Munmap failed");
        exit(-1);
      }
      close(filed2);
      close(fd[0]);
      exit(0);

    default:
      close(fd[0]);
      int filed = open("a.txt", O_RDWR);
      if (filed == -1) {
        perror("Error: unable to open file");
        exit(-1);
      }
      char *region = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, filed, 0);
      if (region == MAP_FAILED) {
        perror("Mmap failed");
        exit(-1);
      }

      memset(region, 5, sizeof(char));
      memset(region + 1, 'h', 1);
      memset(region + 2, 'e', 1);
      memset(region + 3, 'l', 1);
      memset(region + 4, 'l', 1);
      memset(region + 5, 'o', 1);

      if (write(fd[1], "a", 1) < 0) {
        perror("Write failed");
        exit(-1);
      }
      if (munmap(region, 4096) < 0) {
        perror("Munmap failed");
        exit(-1);
      }
      close(filed);
      close(fd[1]);

      int commandStatus;
      while (wait(&commandStatus) != -1) {
        fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(commandStatus));
      }

      if (errno != ECHILD) {
        perror("Wait failed");
        exit(-1);
      }
  }

}

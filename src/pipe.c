#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int pipeTest() {
  int fd[2];
  if (pipe(fd) < 0) {
    perror("Pipe creation failed");
    exit(-1);
  }

  switch (fork()) {
    case -1:perror("Fork failed");
      exit(-1);
    case 0:close(fd[0]);
      dup2(fd[1], STDOUT_FILENO);
      char *args[] = {"cat", "/etc/passwd", NULL};
      execv("/bin/cat", args);
  }

  switch (fork()) {
    case -1:perror("Fork failed");
      exit(-1);
    case 0:close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      char *args[] = {"grep", "/bin/false", NULL};
      execv("/bin/grep", args);
  }

  close(fd[0]);
  close(fd[1]);

  int commandStatus;
  while (wait(&commandStatus) != -1) {
    fprintf(stderr, "Command exited with status %d\n", commandStatus);
  }

  if (errno != ECHILD) {
    perror("Wait failed");
    exit(-1);
  }
}

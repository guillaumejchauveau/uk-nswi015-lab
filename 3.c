#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "2.c"

void test_fork() {
  pid_t f1 = fork();
  if (f1 != 0) {
    printf("PARENT: %d %d\n", getpid(), f1);
  } else {
    printf("CHILD: %d\n", getpid());
  }
}

void exec() {
  char *args[] = {"nano", "/etc/passwd", NULL};
  execv("/bin/nano", args);
}

void fork_me(int depth) {
  printf("PID: %d, PPID: %d\n", getpid(), getppid());
  if (depth == 0) {
    char pid[5];
    snprintf(pid, 5, "%d", getpid());
    char *args[] = {"pstree", "-psa", pid, NULL};
    execv("/usr/bin/pstree", args);
    sleep(1);
    return;
  }
  pid_t f1 = fork();
  if (f1 < 0) {
    perror("fork");
    exit(1);
  } else if (f1 == 0) {
    printf("Fork to depth %d\n", depth - 1);
    fork_me(depth - 1);
  } else {
  }
  sleep(1);
}

void redirect_output_to_file() {
  int fd = openFile("a.txt", O_WRONLY | O_TRUNC, (int) NULL);
  dup2(fd, STDOUT_FILENO);
}

void print_file(char *path) {
  pid_t f1 = fork();
  if (f1 < 0) {
    perror("fork");
    exit(1);
  } else if (f1 == 0) {
    char *args[] = {"wc", "-l", path, NULL};
    execv("/usr/bin/wc", args);
  } else {
  }
}

void print_dir(char *dir) {
  struct dirent *de;
  DIR *dr = opendir(dir);
  if (dr == NULL) {
    perror("Error opening dir");
    exit(1);
  }

  while ((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) {
      continue;
    }
    char fpath[512];
    snprintf(fpath, 512, "%s/%s", dir, de->d_name);
    struct stat fstat;
    if (stat(fpath, &fstat) < 0) {
      perror("File stat error");
      exit(1);
    }
    if (S_ISDIR(fstat.st_mode)) {
      print_dir(fpath);
    } else if (S_ISREG(fstat.st_mode)) {
      print_file(fpath);
    }
  }

  closedir(dr);
}

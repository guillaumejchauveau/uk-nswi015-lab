#include "2.c"
#include <pthread.h>
#include <stdio.h>

void *f() {
  for (int i = 0; i < 20; i++) {
    printf("thread %d\n", i);
  }
  pthread_exit(0);
}

void *th_open_file(char *path) {
  int fd = openFile(path, O_RDONLY, NULL);
  close(fd);
  pthread_exit(0);
}

void *th_output(int file_fd) {
  dup2(STDOUT_FILENO, file_fd);
  pthread_exit(0);
}

int main() {
  pthread_t th1;
  pthread_attr_t th1_attr;

  pthread_attr_init(&th1_attr);
  pthread_attr_setstacksize(&th1_attr, 16000000);

  if (pthread_create(&th1, &th1_attr, f, NULL) == 0) {
    printf("ok\n");
    int rv;
    if (pthread_join(th1, (void **)&rv)) {
      printf("oopsie bis\n");
    }
    printf("done %d\n", rv);
  } else {
    printf("oopsie\n");
  }
}

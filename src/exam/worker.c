#include "counters.h"
#include "filequeue.h"
#include <fcntl.h>
#include <zconf.h>

void *worker() {
  File *file = pop_from_queue();
  if (file == NULL) {
    decrease_worker();
    return NULL;
  }
  int fd = open(file->path, O_RDONLY);
  if (fd == -1) {
    errx(1, "Error: unable to open file %s", file->path);
  }
  size_t zeros_count = 0;
  char t;
  while (read(fd, &t, 1)) {
    if (t == 0) {
      zeros_count++;
    }
  }
  close(fd);
  destroy_file(file);
  increment_counters(1, zeros_count);
  decrease_worker();
  trigger_queue_change();
  return NULL;
}

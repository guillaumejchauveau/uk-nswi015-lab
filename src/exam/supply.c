#include "filequeue.h"
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>

bool supply_done = false;

void *supply() {
  char *path = NULL;
  size_t line_size = 0;
  while (getline(&path, &line_size, stdin) != -1) {
    if (path == NULL) {
      err(-1, "Allocation error");
    }
    path[strcspn(path, "\r\n")] = 0;
    add_file_to_queue(create_file(path));
    line_size = 0;
  }
  free(path);
  supply_done = true;
  return NULL;
}

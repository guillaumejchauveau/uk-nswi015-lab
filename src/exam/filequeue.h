
#include <stdbool.h>
#include <sys/queue.h>

#ifndef UNIX_LAB_FILEQUEUE_H
#define UNIX_LAB_FILEQUEUE_H

typedef struct file File;
struct file {
  TAILQ_ENTRY(file) queue;
  char *path;
};
File *create_file(char *path);
void destroy_file(File *file);
void init_file_queue();
void add_file_to_queue(File *file);
File *pop_from_queue();
bool is_queue_empty();
void wait_for_queue_change();
void done_waiting_queue_change();
void trigger_queue_change();

#endif // UNIX_LAB_FILEQUEUE_H

#include <stddef.h>
#include <pthread.h>
#include "filequeue.h"
#include <err.h>
#include <stdlib.h>

TAILQ_HEAD(tailhead, file) file_queue;
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_change_cond = PTHREAD_COND_INITIALIZER;

File *create_file(char *path) {
  File *file = calloc(1, sizeof(File));
  if (file == NULL) {
    err(-1, "Allocation error");
  }
  file->path = path;
  return file;
}

void destroy_file(File *file) {
  free(file->path);
  free(file);
}

void init_file_queue() {
  TAILQ_INIT(&file_queue);
}

void add_file_to_queue(File *file) {
  pthread_mutex_lock(&queue_lock);
  TAILQ_INSERT_HEAD(&file_queue, file, queue);
  pthread_cond_signal(&queue_change_cond);
  pthread_mutex_unlock(&queue_lock);
}

File *pop_from_queue() {
  pthread_mutex_lock(&queue_lock);
  File *file = TAILQ_FIRST(&file_queue);
  if (file != NULL) {
    TAILQ_REMOVE(&file_queue, file, queue);
  }
  pthread_mutex_unlock(&queue_lock);
  return file;
}

bool is_queue_empty() {
  return TAILQ_EMPTY(&file_queue);
}

void wait_for_queue_change() {
  pthread_cond_wait(&queue_change_cond, &queue_lock);
}
void trigger_queue_change() {
  pthread_mutex_lock(&queue_lock);
  pthread_cond_signal(&queue_change_cond);
  pthread_mutex_unlock(&queue_lock);
}
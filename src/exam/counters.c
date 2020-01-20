#include "counters.h"
#include <pthread.h>
#include <stdio.h>

static size_t files_count = 0;
static size_t zeros_count = 0;
static size_t worker_count = 0;
static pthread_mutex_t counters_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t worker_lock = PTHREAD_MUTEX_INITIALIZER;

size_t get_files_count() {
  pthread_mutex_lock(&counters_lock);
  size_t count = files_count;
  pthread_mutex_unlock(&counters_lock);
  return count;
}
size_t get_zeros_count() {
  pthread_mutex_lock(&counters_lock);
  size_t count = zeros_count;
  pthread_mutex_unlock(&counters_lock);
  return count;
}

void increment_counters(size_t files, size_t zeros) {
  pthread_mutex_lock(&counters_lock);
  files_count += files;
  zeros_count += zeros;
  pthread_mutex_unlock(&counters_lock);
}

void increase_worker() {
  pthread_mutex_lock(&worker_lock);
  worker_count++;
  pthread_mutex_unlock(&worker_lock);
}
void decrease_worker() {
  pthread_mutex_lock(&worker_lock);
  worker_count--;
  pthread_mutex_unlock(&worker_lock);
}
size_t get_worker_count() {
  pthread_mutex_lock(&worker_lock);
  size_t w = worker_count;
  pthread_mutex_unlock(&worker_lock);
  return w;
}
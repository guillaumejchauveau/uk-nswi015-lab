#include "counters.c"
#include "filequeue.c"
#include "supply.c"
#include "worker.c"
#include <err.h>
#include <pthread.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    err(-1, "Expecting 1 argument");
  }
  size_t thread_number = atoi(argv[1]);
  if (thread_number < 3) {
    err(-1, "Program needs at least 3 threads");
  }
  init_file_queue();
  pthread_t supply_thread;
  pthread_create(&supply_thread, NULL, supply, NULL);
  pthread_t worker_thread;

  while (1) {
    wait_for_queue_change();
    while (!is_queue_empty() && get_worker_count() < (thread_number - 2)) {
      increase_worker();
      pthread_create(&worker_thread, NULL, worker, NULL);
    }
    if (is_queue_empty() && supply_done && get_worker_count() == 0) {
      break;
    }
  }
  printf("files: %zu, zeros: %zu\n", get_files_count(), get_zeros_count());
}

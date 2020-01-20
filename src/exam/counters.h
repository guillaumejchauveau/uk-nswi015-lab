#ifndef UNIX_LAB_COUNTERS_H
#define UNIX_LAB_COUNTERS_H

#include <glob.h>

size_t get_files_count();
size_t get_zeros_count();
void increment_counters(size_t files, size_t zeros);
void increase_worker();
void decrease_worker();
size_t get_worker_count();

#endif // UNIX_LAB_COUNTERS_H

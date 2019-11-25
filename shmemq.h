#ifndef SHMEMQ_H
#define SHMEMQ_H

#include <sys/mman.h>

struct _shmemq {
  int fd;

  size_t capacity;
  size_t element_size;
  size_t total_size;

  size_t read_pos;
  size_t write_pos;

  char data[0];
};

typedef struct _shmemq shmemq_t;

shmemq_t *shmemq_create(char *name, size_t capacity, size_t element_size);
void shmemq_destroy(shmemq_t *this);

#endif //SHMEMQ_H

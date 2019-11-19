#ifndef SHMEMQ_H
#define SHMEMQ_H

#include <sys/mman.h>

struct _shmemq {
  char *name;
  int fd;
  size_t max_size;
  char data[0];
};

typedef struct _shmemq shmemq_t;

shmemq_t *shmemq_create(char *name, size_t max_size);
void shmemq_destroy(shmemq_t *this);

#endif //SHMEMQ_H

#ifndef SHMEMQ_H
#define SHMEMQ_H

#include <sys/mman.h>
#include <pthread.h>

/*
 * The following all needs to be in shared memory,
 * since they are shared between processes.
 */
struct _shmemq_data {
    pthread_mutex_t mux;
    size_t references;
    size_t read_pos;
    size_t write_pos;
    char data[0];
};
typedef struct _shmemq_data shmemq_data_t;

/*
 * Definition for shmemq_t.
 */
struct _shmemq {
    int fd;
    char *name;

    size_t capacity;
    size_t element_size;
    size_t total_size;

    shmemq_data_t *data;
};
typedef struct _shmemq shmemq_t;

shmemq_t *shmemq_create(char *name, size_t capacity, size_t element_size);
void shmemq_destroy(shmemq_t *this);

#endif //SHMEMQ_H

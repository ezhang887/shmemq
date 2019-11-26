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

/*
 * Creates a shmemq_t, with the given name, capacity, and element_size.
 * Both capacity and element_size are rounded up to the nearest power of two.
 */
shmemq_t *shmemq_create(char *name, size_t capacity, size_t element_size);

/*
 * Frees the memory used by a shmemq_t.
 */
void shmemq_destroy(shmemq_t *this);

/*
 * Moves element_size bytes from buf to the queue if there is space.
 * Returns -1 if the queue is full, and 0 on success.
 */
int shmemq_push(shmemq_t *this, void *buf);

/*
 * Moves element_size bytes from the queue to buf if the queue has elements.
 * Returns -1 if the queue is empty, and 0 on success.
 */
int shmemq_pull(shmemq_t *this, void *buf);

#endif //SHMEMQ_H

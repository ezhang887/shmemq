#include "shmemq.h"

#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

shmemq_t *shmemq_create(char *name, size_t capacity, size_t element_size){

    int fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror("shm_open(): ");
        return NULL;
    }

    shmemq_t *rv = malloc(sizeof(shmemq_t));
    rv->fd = fd;

    rv->capacity = capacity;
    rv->element_size = element_size;
    rv->total_size = capacity * element_size;
    return rv;
}

void shmemq_destroy(shmemq_t *this){

}

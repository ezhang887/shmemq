#include "shmemq.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

size_t round_up(size_t v){
    v--;
    for(int i=0; i<sizeof(size_t); i++){
        v |= v >> (size_t)pow(2, i);
    }
    v++;
    return v;
}

shmemq_t *shmemq_create(char *name, size_t capacity, size_t element_size){

    int already_exists = 1;

    int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1 && errno == ENOENT){
        already_exists = 0;
        fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    }
    if (fd == -1){
        perror("shm_open(): ");
        return NULL;
    }

    shmemq_t *rv = malloc(sizeof(shmemq_t));
    rv->fd = fd;
    rv->name = strdup(name);

    rv->capacity = round_up(capacity);
    rv->element_size = round_up(element_size);
    rv->total_size = rv->capacity * rv->element_size;

    size_t shmem_size = rv->total_size + sizeof(shmemq_data_t);

    if (ftruncate(fd, shmem_size) == -1){
        perror("ftruncate(): ");
        return NULL;
    }

    rv->data = mmap(NULL, shmem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (rv->data == MAP_FAILED){
        perror("mmap(): ");
        return NULL;
    }

    if (!already_exists){
        rv->data->references = 1;
        rv->data->read_pos = 0;
        rv->data->write_pos = 0;
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&rv->data->mux, &attr);
        pthread_mutexattr_destroy(&attr);
    }
    else {
        rv->data->references++;
    }

    return rv;
}

void shmemq_destroy(shmemq_t *this){
    close(this->fd);

    if (this->data->references == 1){
        shm_unlink(this->name);
    }

    munmap(this->data, this->total_size);

    free(this->name);
    free(this);
}

int shmemq_push(shmemq_t *this, void *buf){
    pthread_mutex_lock(&this->data->mux);

    if (this->data->write_pos - this->data->read_pos >= this->total_size){
        pthread_mutex_unlock(&this->data->mux);
        return -1;
    }

    memmove(&this->data->data[this->data->write_pos & (this->total_size - 1)], buf, this->element_size);
    this->data->write_pos += this->element_size;

    pthread_mutex_unlock(&this->data->mux);
    return 0;
}

int shmemq_pull(shmemq_t *this, void *buf){
    pthread_mutex_lock(&this->data->mux);

    if (this->data->read_pos == this->data->write_pos){
        pthread_mutex_unlock(&this->data->mux);
        return -1;
    }

    memmove(buf, &this->data->data[this->data->read_pos & (this->total_size - 1)], this->element_size);
    this->data->read_pos += this->element_size;
    
    pthread_mutex_unlock(&this->data->mux);
    return 0;
}

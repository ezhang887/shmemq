#include "shmemq.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

static void push(int N){
    shmemq_t *q = shmemq_create("shmemq_test", N, sizeof(int));

    for(int i=0; i<N; i++){
        assert(shmemq_push(q, &i) == 0);
    }

    shmemq_destroy(q);
}

static void pull(int N){
    shmemq_t *q = shmemq_create("shmemq_test", N, sizeof(int));

    for(int i=0; i<N; i++){
        int a;
        if (shmemq_pull(q, &a) != -1){
            assert(a == i);
        }
    }

    shmemq_destroy(q);
}

int main(int argc, char *argv[]){

    int N = 100000000;
    
    int child_pid = fork();
    if (child_pid == -1){
        perror("fork(): ");
        return 1;
    }
    else if (child_pid == 0){
        push(N);
    }
    else {
        pull(N);
    }
}

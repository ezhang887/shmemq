#include "shmemq.h"

#include <stdio.h>

int main(){
    int N = 12;
    shmemq_t *q = shmemq_create("shmemq_test", N, sizeof(int));

    for(int i=0; i<3*N; i++){
        int a = i;
        shmemq_push(q, &a);
    }

    for(int i=0; i<3*N; i++){
        int a;
        if (shmemq_pull(q, &a) != -1){
            printf("%d\n", a);
        }
    }

    shmemq_destroy(q);
}

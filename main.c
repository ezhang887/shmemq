#include "shmemq.h"

#include <stdio.h>

int main(){
    shmemq_t *q = shmemq_create("shmemq_test", 10, sizeof(int));

    int a = 3256;
    shmemq_push(q, &a);
    a = 254;
    shmemq_push(q, &a);

    int b;
    shmemq_pull(q, &b);
    printf("b: %d\n", b);
    shmemq_pull(q, &b);
    printf("b: %d\n", b);

    shmemq_destroy(q);
}

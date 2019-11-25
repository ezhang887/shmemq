#include "shmemq.h"

int main(){
    shmemq_t *q = shmemq_create("shmemq_test", 10, sizeof(int));
    shmemq_destroy(q);
}

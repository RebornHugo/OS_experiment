#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

float res = 0;
struct arg_struct {
    int arg1;
    int arg2;
};

void *compute(void *arg) {
    struct arg_struct *me;
    me = (struct arg_struct *) arg;
    int start = me->arg1;
    int end = me->arg2;
    for (; start <= end; start++)
        res += powf(-1, start + 1) / (2 * start - 1);
}

int main() {
    int n = 23;
    pthread_t worker_tid;
    struct arg_struct *parent = malloc(sizeof(struct arg_struct));
    struct arg_struct *child = malloc(sizeof(struct arg_struct));
    parent->arg1 = 1;
    parent->arg2 = n / 2;
    child->arg1 = n / 2 + 1;
    child->arg2 = n;
    pthread_create(&worker_tid, NULL, &compute, child);
    compute(parent);
    pthread_join(worker_tid, NULL);
    printf("%f", res * 4);
    return 0;
}


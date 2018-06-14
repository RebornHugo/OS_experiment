#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#define NR_CPU 4

struct arg_struct {
    int start;
    int end;
};
struct result {
    float sum;
};

void *compute(void *arg) {
    struct arg_struct *me;
    me = (struct arg_struct *) arg;
    int start = me->start;
    int end = me->end;
    float sum = 0;
    for (; start <= end; start++)
        sum += powf(-1, start + 1) / (2 * start - 1);
    struct result *res;
    res = malloc(sizeof(struct result));
    res->sum = sum;
//    printf("**  %f\n", res->sum);
    return res;
}

int main() {
    int n = 20; //add to n
    int i;
    pthread_t workers[NR_CPU];
    struct arg_struct args[NR_CPU];
    int p = 1;
    for (i = 0; i < NR_CPU; i++) {
        struct arg_struct *arg;
        arg = &args[i];
        arg->start = p;
        if (p + n / NR_CPU - 1 <= n) {
            arg->end = p + n / NR_CPU - 1;
        } else {
            arg->end = n;
            break;
        }
        pthread_create(&workers[i], NULL, &compute, arg);
        p += (n / NR_CPU);
    }


    float sum = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **) &result);
        sum += result->sum;
//        printf("result->sum = %d\n", result->sum);
        free(result);
    }
    printf("pi = %f\n", sum * 4);
    return 0;
}



#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define N 8

struct param
{
    int a;
};

typedef struct
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

int count = 1;
void *compute(void *arg)
{
    count += 1;
    struct param *para;
    para = (struct param *)arg;
    para->a += 1;
    if (count < N)
    {
        pthread_t next_id;
        pthread_create(&next_id, NULL, compute, para);
        pthread_join(next_id, NULL);
        return NULL;
    }
    else
    {
        printf("Finally, the first thread receives %d", para->a);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t start_id;
    struct param *para;
    para = (struct param *)malloc(sizeof(struct param));
    para->a = 1;
    struct para *res;
    // res = (struct param *)compute(para);
    pthread_create(&start_id, NULL, compute, para);
    pthread_join(start_id, NULL);
    return 0;
}

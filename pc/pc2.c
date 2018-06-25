#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in1;
int in2;
int out1;
int out2;

void put_item_to_buffer1(int item)
{
    buffer1[in1] = item;
    in1 = (in1 + 1) % CAPACITY;
}

void put_item_to_buffer2()
{

    buffer2[in2] = buffer1[out1] - ('a' - 'A');

    // logging
    printf("  compute item:%c to %c\n", buffer1[out1], buffer2[in2]);

    out1 = (out1 + 1) % CAPACITY;
    in2 = (in2 + 1) % CAPACITY;
}

int get_item_from_buffer2()
{
    int item;
    item = buffer2[out2];
    out2 = (out2 + 1) % CAPACITY;
    return item;
}

typedef struct
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex1_sema;
sema_t mutex2_sema;
sema_t empty_buffer1_sema;
sema_t empty_buffer2_sema;
sema_t full_buffer1_sema;
sema_t full_buffer2_sema;

#define ITEM_COUNT (CAPACITY * 2)

void *compute(void *arg)
{
    int i;

    for (i = 0; i < ITEM_COUNT; i++)
    {

        sema_wait(&full_buffer1_sema);
        sema_wait(&empty_buffer2_sema);

        sema_wait(&mutex1_sema);
        sema_wait(&mutex2_sema);

        put_item_to_buffer2();

        sema_signal(&mutex1_sema);
        sema_signal(&mutex2_sema);

        sema_signal(&empty_buffer1_sema);
        sema_signal(&full_buffer2_sema);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++)
    {
        sema_wait(&full_buffer2_sema);
        sema_wait(&mutex2_sema);

        item = get_item_from_buffer2();
        printf("       consume item: %c\n", item);

        sema_signal(&mutex2_sema);
        sema_signal(&empty_buffer2_sema);
    }

    return NULL;
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++)
    {
        sema_wait(&empty_buffer1_sema);
        sema_wait(&mutex1_sema);

        item = i + 'a';
        put_item_to_buffer1(item);
        printf("produce item: %c\n", item);

        sema_signal(&mutex1_sema);
        sema_signal(&full_buffer1_sema);
    }

    return NULL;
}

int main()
{
    pthread_t computer_tid;
    // pthread_t producer_tid;
    pthread_t consumer_tid;

    sema_init(&mutex1_sema, 1);
    sema_init(&mutex2_sema, 1);
    sema_init(&empty_buffer1_sema, CAPACITY - 1);
    sema_init(&empty_buffer2_sema, CAPACITY - 1);
    sema_init(&full_buffer1_sema, 0);
    sema_init(&full_buffer2_sema, 0);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();

    pthread_join(consumer_tid, NULL);
    pthread_join(computer_tid, NULL);
    return 0;
}
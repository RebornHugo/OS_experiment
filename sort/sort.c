//
// Created by hugo on 18-6-14.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LEN 12

struct param {
    int *array;
    int start;
    int end;
};




void *bubbleSort(void *arg) {
    struct param *p = (struct param *) arg;
    int i, j, temp;
    for (i = p->start; i < p->end; i++)
        // Last i elements are already in place
        for (j = p->start; j < p->end - (i - p->start); j++) {
//            int qwewer = 123;
            if (p->array[j] > p->array[j + 1]) {
                temp = p->array[j];
                p->array[j] = p->array[j + 1];
                p->array[j + 1] = temp;
            }
        }
}


/* Function to print an array */
void printArray(int A[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

int *merge(int arr[], int mid, int end) {
    int i = 0, j = mid + 1, p = 0;
//    int arr2[end + 1];
    static int arr2[LEN];
    while (i <= mid && j <= end) {
        if (arr[i] <= arr[j]) {
            arr2[p++] = arr[i++];
        } else if (arr[i] > arr[j]) { arr2[p++] = arr[j++]; }
    }
    if (i > mid && j <= end) {
        for (; j <= end; ++j) {
            arr2[p++] = arr[j];
        }
    } else if (j > end && i <= mid) {
        for (; i <= mid; i++) {
            arr2[p++] = arr[i];
        }
    }
    return arr2;
}

/* Driver program to test above functions */
int main() {
    pthread_t worker_tid;
    int arr[] = {12, 45, 9, 0, 54, 43, 11, 13, 5, 7, 6, 2};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    printf("Given array is \n");
    printArray(arr, arr_size);


    int start_main = 0, end_main = (arr_size - 1) / 2;
    int start_aux = (arr_size - 1) / 2 + 1, end_aux = arr_size - 1;

    struct param *arg_main = malloc(sizeof(struct param));
    arg_main->array = arr;
    arg_main->start = start_main, arg_main->end = end_main;

    struct param *arg_aux = malloc(sizeof(struct param));
    arg_aux->array = arr;
    arg_aux->start = start_aux, arg_aux->end = end_aux;

    pthread_create(&worker_tid, NULL, &bubbleSort, arg_main);
    bubbleSort(arg_aux);
    pthread_join(worker_tid, NULL);

    printArray(arr, arr_size);
    free(arg_aux);
    free(arg_main);


    int *res = merge(arr, end_main, end_aux);
    printf("\nSorted array is \n");
    printArray(res, arr_size);

    return 0;
}

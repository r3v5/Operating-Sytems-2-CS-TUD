#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint64_t tid;
    int result;
} thread_data_t;


void* worker(void* arg) {
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    int* input = (int*)arg;
    thread_data_t* data = malloc(sizeof(thread_data_t));

    if (data == NULL) {
        perror("Failed to allocate memory");
        pthread_exit(NULL);
    }

    data->tid = tid;              // Store the thread ID
    data->result = (*input) * 2;  // Perform some computation

    // Display the thread ID (cast pthread_t to unsigned long for printing)
    printf("Worker thread: ID = %llu, computed result = %d\n", tid, data->result);
    
    pthread_exit((void*)data); // Return the dynamically allocated result
}

void* super_worker(void* arg)
{
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    int* input = (int*)arg;
    thread_data_t* data = malloc(sizeof(thread_data_t));

    if (data == NULL) {
        perror("Failed to allocate memory");
        pthread_exit(NULL);
    }

    data->tid = tid;              // Store the thread ID
    data->result = (*input) * (*input);  // Perform some computation

    // Display the thread ID (cast pthread_t to unsigned long for printing)
    printf("Super Worker thread: ID = %llu, computed result = %d\n", tid, data->result);
    
    pthread_exit((void*)data); // Return the dynamically allocated result
}

int main() {
    pthread_t thread;
    pthread_t super_thread;
    int value = 10;
    void* return_value;
    void* final_value;
    uint64_t main_tid;
    pthread_threadid_np(NULL, &main_tid); 
    thread_data_t* worker_data;
    thread_data_t* super_worker_data;

    // Display the Master thread ID (cast pthread_t to unsigned long for printing)
    printf("Master thread: ID = %llu, initial result = %d\n", main_tid, value);


    // Create a Worker thread
    if (pthread_create(&thread, NULL, worker, &value) != 0) {
        perror("Failed to create thread");
        exit(EXIT_FAILURE);
    }

    // Wait for the thread to finish
    if (pthread_join(thread, (void**)&worker_data) != 0) {
        perror("Failed to join thread");
        exit(EXIT_FAILURE);
    }

    int intermediate_result = worker_data->result;
    uint64_t worker_tid = worker_data->tid;
    free(worker_data);

    if (pthread_create(&super_thread, NULL, super_worker, &intermediate_result) != 0)
    {
        perror("Failed to create thread");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(super_thread, (void**)&super_worker_data) != 0) {
        perror("Failed to join thread");
        exit(EXIT_FAILURE);
    }

    uint64_t super_worker_tid = super_worker_data->tid;
    int super_worker_result = super_worker_data->result;
    free(super_worker_data);

    // Print results and thread IDs
    printf("Master thread: Received result = %d from Worker TID = %llu\n", intermediate_result, worker_tid);
    printf("Master thread: Received result = %d from Super Worker TID = %llu\n", super_worker_result, super_worker_tid);

    return 0;
}
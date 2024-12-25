#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *signal_thread(void *t) {
    int i;
    long my_id = (long)t;

    for (i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;

        if (count == COUNT_LIMIT) {
            printf("Signal Thread: thread %ld, count = %d. Threshold reached.\n",
                   my_id, count);
            pthread_cond_signal(&count_threshold_cv);
        }

        pthread_mutex_unlock(&count_mutex);
        sleep(1);  // Suspend thread
    }
    pthread_exit(NULL);
}

void *wait_thread(void *t) {
    long my_id = (long)t;

    printf("Starting wait function. Thread ID is %ld\n", my_id);

    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        printf("Wait Thread: thread %ld, Count = %d. Going into wait...\n", my_id, count);

        pthread_cond_wait(&count_threshold_cv, &count_mutex);

        count += 125;
        printf("Wait Thread: thread %ld, count now = %d.\n", my_id, count);
    }

    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int rc;
    pthread_t threads[NUM_THREADS];

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    /* Create threads */
    rc = pthread_create(&threads[0], NULL, wait_thread, (void *)1);
    if (rc) {
        printf("ERROR: return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    sleep(1);

    rc = pthread_create(&threads[1], NULL, signal_thread, (void *)2);
    if (rc) {
        printf("ERROR: return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&threads[2], NULL, signal_thread, (void *)3);
    if (rc) {
        printf("ERROR: return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    /* Wait for all threads to complete */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Final value of count = %d.\n", count);

    /* Clean up and exit */
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);

    pthread_exit(NULL);
}

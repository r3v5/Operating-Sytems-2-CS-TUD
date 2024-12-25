#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*

 Compile: gcc -o Mutex1 mutex1.c -lpthread
Run: ./Mutex1
Results:

    Counter value: 1
    Counter value: 2

When a mutex lock is attempted against a mutex which is held by another thread, the thread is blocked until the mutex is unlocked. 
When a thread terminates, the mutex does not unless explicitly unlocked. Nothing happens by default.

*/

void *functionC(void *threadID); // Correct prototype
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main() {
    int rc1, rc2;
    long t1 = 1, t2 = 2;
    pthread_t thread1, thread2;

    /* Create independent threads each of which will execute functionC */
    if ((rc1 = pthread_create(&thread1, NULL, &functionC, (void *)t1))) {
        printf("Thread creation failed: %d\n", rc1);
    }
    printf("Main thread: pthread_t for thread1 = %p\n", (void *)thread1);

    if ((rc2 = pthread_create(&thread2, NULL, &functionC, (void *)t2))) {
        printf("Thread creation failed: %d\n", rc2);
    }
    printf("Main thread: pthread_t for thread2 = %p\n", (void *)thread2);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(0);
}

void *functionC(void *threadID) {
    long thread_num;
    thread_num = (long)threadID;
    printf("Thread %ld is accessing before mutex\n", thread_num);
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("Counter value: %d\n", counter);
    pthread_mutex_unlock(&mutex1);
    printf("Thread %ld is finished function\n", thread_num);

    return NULL; // Explicit return statement
}

/******************************************************************************
*  The Wait and Signal is one approach to ensure thread synchronisation and prevent the race problem
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *signal_fnt(void *t) 
{
  int i;
  long my_id = (long)t;

  for (i=0; i < TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    count++;

    /* 
    Check the value of count and signal waiting thread when condition is
    reached.  Note that this occurs while mutex is locked. 
    */
    if (count == COUNT_LIMIT) {
      printf("Signal Thread: thread %ld, count = %d  Threshold reached. ",
             my_id, count);
      pthread_cond_signal(&count_threshold_cv);
      printf("Just sent signal.\n");
      }
    printf("Signal thread: thread %ld, count = %d, unlocking mutex\n", 
	   my_id, count);
    pthread_mutex_unlock(&count_mutex);

    /* Do some work so threads can alternate on mutex lock */
    sleep(1);
    }
  pthread_exit(NULL);
}

void *wait_fnt(void *t) 
{
  long my_id = (long)t;

  printf("Starting watch_count(): thread %ld\n", my_id);

  /*
  Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
  will automatically and atomically unlock mutex while it waits. 
  
  Also, note that if COUNT_LIMIT is reached before this routine is run by
  the waiting thread, the loop will be skipped to prevent pthread_cond_wait
  from never returning.
  */
  pthread_mutex_lock(&count_mutex);
  while (count < COUNT_LIMIT) {
		printf("watch_count(): thread %ld Count= %d. Going into wait...\n", my_id,count);
		
	pthread_cond_wait(&count_threshold_cv, &count_mutex);            // the thread is forced to wait and unlocks the mutex
		printf("Wait thread: thread %ld Condition signal received. Count= %d\n", my_id,count);
		printf("Wait thread: thread %ld Updating the value of count...\n", my_id);
		
    count += 125;
		printf("Wait thread: thread %ld count now = %d.\n", my_id, count);
    }
  
  printf("Wait thread: thread %ld Unlocking mutex.\n", my_id);
 
  // unlock the mutex
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int i, rc; 
  long t1=1, t2=2, t3=3;
  pthread_t threads[3];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

  /* For portability, explicitly create threads in a joinable state */
  /*this piece of code is not essential */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  
  //create Three threads one to use the wait function and two for the signal function 
  pthread_create(&threads[0], &attr, wait_fnt, (void *)t1);
  pthread_create(&threads[1], &attr, signal_fnt, (void *)t2);
  pthread_create(&threads[2], &attr, signal_fnt, (void *)t3);

  /* Wait for all 3 threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n", 
          NUM_THREADS, count);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  
  pthread_exit (NULL);

}


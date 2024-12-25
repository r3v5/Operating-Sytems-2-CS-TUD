#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#define BUFF_SIZE 10    // global variable for size of buffer 

char buffer[BUFF_SIZE];   // the bounded buffer - to be implemented as a circular buffer
int producerIndex = 0;    //producer index pointer 
int consumerIndex = 0;   // consumer index pointer

int Times;

// declare semaphore variables as global variables

sem_t EmptyPositions;    // semaphore for empty positions
sem_t FullPositions;  // semaphore for full positions
sem_t BufferLock;   // semaphore for critical section lock

//  The Insert function  producer function 
void Insert(char item)
{
    // show the item that was produced
    printf("Producing %c ...\n", item);

    //  Code to add items to a bounder buffer: The PRODUCER ALGORITHM ; 
  
    // INSERT CODE for the p function: sem_wait
    sem_wait(&EmptyPositions);
    sem_wait(&BufferLock);  // enter critical section

    // the producer function is entering the critical region
    buffer[producerIndex] = item;    // put the value produced into the bounder buffer array

    //INSERT CODE  to increment bounded buffer index
    producerIndex = (producerIndex + 1) % BUFF_SIZE;

   // INSERT CODE the V function for the lock and the full mutexs  
    sem_post(&BufferLock);
    sem_post(&FullPositions);
     
}
 
 //The producer thread used to call 10 a producer function 15 times. 
  void * Prod ()
  {
    int i;

    // code insert Times into buffer
    for(i = 0; i < Times; i++)
    {
      Insert((char)('A'+ i % 26));   // Call the Put (producer algorithm) function passing a to it an alphabetical character 
    }
    
     // exit the thread gracefully 
       pthread_exit( NULL );
       return NULL;
  }

/* Insert code for the consumer thread: */

void Delete()
{

    char item ;
    int i;

      // INSERT CODE FOR P FUNCTIONS to consume from the buffer     
          sem_wait(&FullPositions);
          sem_wait(&BufferLock);
            
          //INSERT CODE TO Remove an item from the buffer and increment index
          item = buffer[consumerIndex];
          consumerIndex = (consumerIndex + 1) % BUFF_SIZE;
      
    // INSERT CODE  FOR THE V FUNCTIONS to consume from the buffer
            sem_post(&BufferLock);
            sem_post(&EmptyPositions);
        
      
 
    printf("consuming letter %c ...\n", item);




}

// the consumer thread: this calls the consumer algorithm function (Get) 10 times 
void * Consume()
{
  
    
   int i;
       
    // Code to remove items from the buffer: it is called a number of Times
    for(i = 0; i < Times; i++)
   {
      
       //   INSERT CODE to consume an item from the bounder buffer
         Delete(); 
   }

                                                                           
       pthread_exit( NULL );
       return NULL;


}

//   second command line argument is the size of the bounder buffer
int main(int argc, char** argv)
{
    
       pthread_t idP, idC;       // thread ID variables
      
       int rc1, rc2;
       
       
       // comment out this code for part C
      Times = 15;    // Variable to control the number of times items are added/ removed from buffer  
     

     
     // Part C
     // INSERT Code to input the number of times form command line include error prevention. 
     if (argc != 2)
    {
        printf("Usage: %s <number_of_items>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Times = atoi(argv[1]);
    if (Times <= 0)
    {
        printf("Error: number_of_items must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
       
     // INSERT CODE TO INITALISE the SEMAPHOREs for the producer consumer algorithm   
    sem_init(&EmptyPositions, 0, BUFF_SIZE);
    sem_init(&FullPositions, 0, 0);
    sem_init(&BufferLock, 0, 1);
    
    // create thread for producer
    
    if( (rc1=pthread_create( &idP, NULL, Prod, NULL)) )
        {
            printf("Thread creation failed: %d\n", rc1);
        }
    
    
    //INSERT CODE TO CREATE THREAD FOR CONSUMER 
    if ((rc2 = pthread_create(&idC, NULL, Consume, NULL)))
    {
        printf("Thread creation failed: %d\n", rc2);
        exit(EXIT_FAILURE);
    }
   
   
     
    //INSERT CODE THAT SYNCHRONISES WORKER THREADS AND MASTER THREAD (PROCESS)
    pthread_join(idP, NULL);
    pthread_join(idC, NULL);
            
   
    // CODE TO DESTROY ALL THE THREE SEMAPHORES
      sem_destroy( &FullPositions );
      sem_destroy( &EmptyPositions );
      sem_destroy (&BufferLock);           
            
  
    printf("exiting program\n");      
    return 0;
}

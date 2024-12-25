/* ----------------------------------------------------------------- */
/*   
 *
 *   LabTest Create Two Children: relate sample code can nbe found in the lecture covered in week 2
 *
 *   	Child 1 Calculates the Factorial of the second coomand line argument:
 *
 */    

/* ----------------------------------------------------------------- */

#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define   MAX_COUNT  10
#define   BUF_SIZE   100

void  ChildProcess1(char*);    /* child process 1 prototype  */
void ChildProcess2(char[]);   /* child process 2 prototype of */ 
void  ParentProcess(char*);               /* parent process prototype */

int  main(int argc, char** argv)
{
     pid_t   pid1, pid2, pid;
     int     status;
     int     i;
    
     
   

     /*check number of command line arguments */

     if (argc != 3)
	{
	 printf("invalid number of arguments about to exit!!!!!!!!!!!!\n");
	 exit(0);
	}


     printf("*** Parent is about to fork process 1 ***\n");
     if ((pid1 = fork()) < 0) {
          printf("Failed to fork process 1\n");
          exit(1);
     }
     else if (pid1 == 0) 
          ChildProcess1(argv[1]);

     printf("*** Parent is about to fork process 2 ***\n");
     if ((pid2 = fork()) < 0) {
          printf("Failed to fork process 2\n");
          exit(1);
     }
     else if (pid2 == 0) 
          ChildProcess2(argv[2]);

     ParentProcess(argv[0]);
    
     
     printf("\n*** Program exits ***\n");
    
    // CODE TO TERMINATE ANY PROCESS: PARENT or CHILD 
     exit(0);

     return 0;
}

/* ----------------------------------------------------------------- */
/* FUNCTION  ParentProcess :                                         */
/*   prints the name of the exe file                                  */
/* ----------------------------------------------------------------- */



void  ParentProcess(char* Ptr)
{
     pid_t pid;
    int  count = 0, status;

    printf( "\n*** Parent enters waiting status .....\n");
    pid = wait(&status);
    printf( "\n*** Parent detects process %d was done ***\n", pid);
   
  
     pid = wait(&status);
     printf("*** Parent detects process %d is done ***\n", pid);
     printf("\nThe Parent process begins...\nThe File name of the executable program is: ");


   
    printf("this parent is intransigent and has decided not to to as requested..\n");
	
    pid = getpid();

    printf("\nthe parent process pid number %d is finishing \n",pid);
     
     
}
 

/* calculates the factorialof the second command line argument and prints child ID*/
void  ChildProcess1(char *arg2)
{
     pid_t  pid;
     int    factorial =1, second;


   
   // convert second argument to integer

    second = atoi(arg2);	
     
    pid = getpid();

    printf("\nThe First Child Begins:....");	
   // printf("the value of the second command line argument is: %d\n",second);	

   /* calculate the factorial of second argument*/

    while (second > 1)
      {	
	factorial = factorial*second;
	second = second -1;
      } 	

       
     printf("\nthe factorial of the second argument is: %d", factorial);
     printf("\nThe first child process 1  (pid = %d) is about to exit\n",pid);
     exit(0);
}


/* calutes the cube of the 3rd command line argument and prints the child ID */
void  ChildProcess2(char *arg3)
{
     pid_t  pid;
     int    i, third;

     
     pid = getpid();
    
    third = atoi(arg3); 	
     
    printf("\nThe second Child begins....\n");
    printf("the cube of the 3rd coomand line arguments %d is %d\n", third, third*third*third); 
         
         
     printf("the second child process (pid = %d)  exits\n", pid);
  
     exit(0);
}





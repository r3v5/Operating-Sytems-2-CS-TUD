/* ----------------------------------------------------------------- */
/* PROGRAM  shell.c                                                  */
/*    This program reads in an input line, parses the input line     */
/* into tokens, and use execvp() to execute the command.             */
/* 
 
 found at: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html

a similiar program in C++ can be found at: http://web.mst.edu/~ercal/284/UNIX-fork-exec/Fork-Exec-2.cpp

 
 
	sample input ./exec  (run an exe file in current directory) 																	*/

/* ----------------------------------------------------------------- */

#include  <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/* ----------------------------------------------------------------- */
/* FUNCTION  parse:                                                  */
/*    This function takes an input line and parse it into tokens.    */
/* It first replaces all white spaces with zeros until it hits a     */
/* non-white space character which indicates the beginning of an     */
/* argument.  It saves the address to argv[], and then skips all     */
/* non-white spaces which constitute the argument.                   */
/* ----------------------------------------------------------------- */

void  parse(char *line, char **argv)
{
    
//     printf("test parse..\n");	

     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
    

	 *argv = '\0';                 /* mark the end of argument list  */

//	printf("end parse\n");


}

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* ----------------------------------------------------------------- */
     
void  execute(char **argv)
{
     pid_t  pid;
     int    status;
     
 //    printf("test execute fnt\n");	

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(42);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
		  printf(" before the wait command \n");
          while (wait(&status) != pid);       /* wait for completion  */
          printf (" after the wait command \n");
     }

	printf(" end execute fnt\n");
}

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
     
void  main(void)
{
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */
     
     while (1) {                   /* repeat until done ....         */
          printf("Shell (enter exit to finish) -> ");     /*   display a prompt             */
         fgets(line,1024,stdin);              /*   read in the command line     */
//          printf("\ntest %s\n",line);
          parse(line, argv);       /*   parse the line               */
          if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
          { 	printf("exit program");	
               exit(0);            /*   exit if it is                */
	  }	
          execute(argv);           /* otherwise, execute the command */
     }

	printf("end main fnt\n");
}

                


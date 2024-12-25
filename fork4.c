#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void Square(int number) {
    printf("Child 1 process: The square of %d is %d\n", number, number * number);
}

void Cube(int number) {
    printf("Child 2 process: The cube of %d is %d\n", number, number * number * number);
}

void First(int number) {
    printf("Parent process: The first argument is %d\n", number);
}

int main(int argc, char **argv) {
    printf("--beginning of program\n");

    if (argc != 3) {
        printf("Number of command-line arguments should be 3\n");
        printf("Exiting program...\n");
        exit(0);
    }

    pid_t pid1, pid2;

    printf("Parent process: About to fork the first child...\n");
    pid1 = fork();

    if (pid1 == 0) {
        // First child process
        printf("Child 1 process: Fork successful. PID = %d\n", getpid());
        int arg2 = atoi(argv[2]); // Convert second argument to an integer
        Square(arg2);
        printf("Child 1 process: Exiting...\n");
        exit(0);
    } else if (pid1 > 0) {
        // Parent process
        printf("Parent process: Forked the first child. PID = %d\n", pid1);

        printf("Parent process: About to fork the second child...\n");
        pid2 = fork();

        if (pid2 == 0) {
            // Second child process
            printf("Child 2 process: Fork successful. PID = %d\n", getpid());
            int arg2 = atoi(argv[2]); // Convert second argument to an integer
            Cube(arg2);
            printf("Child 2 process: Exiting...\n");
            exit(0);
        } else if (pid2 > 0) {
            // Parent process
            printf("Parent process: Forked the second child. PID = %d\n", pid2);

            int arg1 = atoi(argv[1]); // Convert first argument to an integer
            First(arg1);

            printf("Parent process: Waiting for child processes to finish...\n");

            int status;
            pid_t child_pid;

            // Wait for each child process
            for (int i = 0; i < 2; i++) {
                child_pid = wait(&status);
                if (child_pid == pid1) {
                    printf("Parent process: First child finished.\n");
                } else if (child_pid == pid2) {
                    printf("Parent process: Second child finished.\n");
                }
            }
        } else {
            // Fork failed for the second child
            printf("Parent process: Fork failed for the second child!\n");
            return 1;
        }
    } else {
        // Fork failed for the first child
        printf("Parent process: Fork failed for the first child!\n");
        return 1;
    }

    printf("--end of program--\n");
    return 0;
}
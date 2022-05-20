#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -g -Wall -o pipe2 pipe2.c
int main(void) {
    int pd[2];
    // create a pipe
    if (pipe(pd)) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    printf("pd = [%d, %d]\n", pd[0], pd[1]);

    int child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    // fork a child to share the pipe
    if (child) {
        printf("parent %d close pd[0]\n", getpid());
        close(pd[0]); 
        for (int i = 0; i < 10; i++) {
            printf("parent %d writing to pipe\n", getpid());
            int n = write(pd[1], "I AM YOUR PAPA", 16);
            printf("parent %d wrote %d bytes to pipe\n", getpid(), n);
        }
        printf("parent %d exit\n", getpid());
        close(pd[1]);
    } else {
        printf("child %d close pd[1]\n", getpid());
        close(pd[1]);
        for (int i = 0; i < 10; i++) {
            char line[256];
            printf("child %d reading from pipe\n", getpid());
            int n = read(pd[0], line, 255);
            if (n <= 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            line[n] = 0;
            printf("child read %d bytes from pipe: %s\n", n, line);
        }
        close(pd[0]);
    }

    exit(EXIT_SUCCESS);
}

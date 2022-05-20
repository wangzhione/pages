#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int pd[2], n, i;
char line[256];

// gcc -g -Wall -o pipe pipe.c
int main() 
{
    pipe(pd);           // create a pipe
    printf("pd = [%d, %d]\n", pd[0], pd[1]);
    if (fork()) {       // fork a child to share the pipe
        printf("parent %d close pd[0]\n", getpid());
        close(pd[0]);   // parent as pipe WRITE
        while (i++ < 10) {
            printf("parent %d writing to pipe\n", getpid());
            n = write(pd[1], "I AM YOUR PAPA", 16);
            printf("parent %d wrote %d bytes to pipe\n", getpid(), n);
        }
        printf("parent %d exit\n", getpid());
    } else {
        printf("child %d close pd[1]\n", getpid());
        close(pd[1]);
        while (i++ < 10) {
            printf("child %d reading from pipe\n", getpid());
            if ((n = read(pd[0], line, 128))) {
                line[n] = 0;
                printf("child read %d bytes from pipe: %s\n", n, line);
            } else 
                exit(0);
        }
    }
}
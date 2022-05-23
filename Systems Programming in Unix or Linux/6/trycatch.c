#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>
#include <stdatomic.h>

#include <unistd.h>

#ifndef UNUSED
#define UNUSED(parameter) if (parameter) {}
#endif//UNUSED

static jmp_buf env;
static atomic_int count;

static void BAD(void) {
    int * ip = NULL;
    
    printf("in BAD(): try to dereference NULL pointer\n");

    *ip = 123;

    printf("should not see this line\n");
}

static void handler_sigsegv(int sig, siginfo_t * info, void * ucontext) {
    UNUSED(ucontext);

    int n = atomic_fetch_add(&count, 1)+1;

    // [error] 不应该在信号中使用 printf 等不可重入函数
    printf("handler: sig = %d from PID = %d, UID = %d count = %d\n", sig, info->si_pid, info->si_uid, n);

    if (n >= 4) {
        longjmp(env, 1234);
    }
}

// gcc -O2 -g -Wall -Wextra -Werror -o trycatch trycatch.c
int main(void) {
    int res;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = handler_sigsegv;
    act.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGSEGV, &act, NULL)) {
        perror("sigaction SIGSEGV : ");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        if ((res = setjmp(env)) == 0) {
            BAD();
        } else {
            printf("proc %d survived SEGMENTATION FAULT: res = %d\n", getpid(), res);
            break;
        }
    }

    printf("proc %d looping\n", getpid());
    exit(EXIT_SUCCESS);
}

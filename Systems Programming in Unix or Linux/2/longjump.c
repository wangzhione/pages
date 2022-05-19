/** longjump.c file: demonstrate long jump in Linux  */
#include <stdio.h>
#include <setjmp.h>

jmp_buf env; // for saving longjmp environment

void B() {
    printf("enter B()\n");
    printf("long jump? (y | n) ");
    if (getchar() == 'y') {
        // longjmp 0 之后 setjmp 也会 return 1
        longjmp(env, 0);
    }
    printf("exit B()\n");
}

void A() {
    printf("enter A()\n");
    B();
    printf("exit A()\n");
}

// gcc -g -Wall -o longjump longjump.c 
int main(void) {
    int r, a = 100;
    printf("call setjmp to save environment\n");
    if ((r = setjmp(env)) == 0) {
        A();
        printf("normal return\n");
    } else {
        printf("back to main() via long jump, r=%d a=%d\n", r, a);
    }
    return 0;
}

#include <stdio.h>

extern int B();

// cc -S a.c
int A(int x, int y) {
    int d, e, f;
    d = 4; e =5; f = 6;
    f = B(d, e);
    return 0;
}

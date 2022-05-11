#include <stdlib.h>

static void sort_shell_insert(int a[], int len, int delta) {
    for (int i = delta; i < len; i += delta) {
        int j = i;
        int anchor = a[j];
        while (j >= delta && anchor < a[j-delta]) {
            a[j] = a[j-delta];
            j -= delta;
        }
        a[j] = anchor;
    }
}

void sort_shell(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int delta = 1;
    while (delta < len / 3) {
        // O(n^(3/2)) by Knuth 1973
        // 1 4 13 40 121 ...
        delta = delta * 3 + 1;
    }

    for (; delta >= 1; delta /= 3) {
        sort_shell_insert(a, len, delta);
    }
}

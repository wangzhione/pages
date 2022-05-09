#include <stdlib.h>
#include <stdbool.h>

void sort_bubble(int a[], int len) {
    for (; len > 1; len--) {
        for (int i = 1; i < len; i++) {
            if (a[i-1] > a[i]) {
                int temp = a[i]; a[i] = a[i-1]; a[i-1] = temp;
            }
        }
    }
}

void sort_bubble_flag(int a[], int len) {
    for (; len > 1; len--) {
        bool flag = true;
        for (int i = 1; i < len; i++) {
            if (a[i-1] > a[i]) {
                int temp = a[i]; a[i] = a[i-1]; a[i-1] = temp;
                flag = false;
            }
        }
        if (flag) {
            break;
        }
    }
}

#include <stdlib.h>
#include <stdbool.h>

void sort_bubble(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    for (int i = len; i > 1; i--) {
        for (int j = 1; j < i; j++) {
            if (a[j-1] > a[j]) {
                int tmp = a[j-1];
                a[j-1] = a[j];
                a[j] = tmp;
            }
        }
    }
}

void sort_bubble_upgrade(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    bool keep = true;
    for (int j = len; j > 1 && keep; j--) {
        keep = false;
        for (int i = 1; i < j; i++) {
            if (a[i-1] > a[i]) {
                keep = true;

                int tmp = a[i];
                a[i] = a[i-1];
                a[i-1] = tmp;
            }
        }
    }
}

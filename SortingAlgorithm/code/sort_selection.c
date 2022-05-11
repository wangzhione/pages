#include <stdlib.h>

void sort_selection(int a[], int len) {
    // i 控制循环次数 len-2 - 0 + 1 = len - 1
    for (int i = 0; i < len-1; i++) {
        int x = i;
        int m = a[x];
        for (int j = i+1; j < len; j++) {
            if (m > a[j]) {
                m = a[x = j];
            }
        }
        if (x != i) {
            a[x] = a[i];
            a[i] = m;
        }
    }
}

void sort_selection1(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    } 

    // i 控制循环次数 len-2 - 0 + 1 = len - 1
    for (int i = 0; i < len-1; i++) {
        int idx = i;
        int miv = a[idx];
        for (int j = i+1; j < len; j++) {
            if (miv > a[j]) {
                miv = a[idx = j];
            }
        }
        if (idx != i) {
            a[idx] = a[i];
            a[i] = miv;
        }
    }
}

void sort_selection2(int a[], int len) {
    // len - 1 - 1 + 1 = len - 1 控制循环次数
    while (--len >= 1) {
        int x = 0;
        int m = a[x];
        for (int i = 1; i <= len; i++) {
            if (m < a[i]) {
                m = a[x = i];
            }
        }
        if (x != len) {
            a[x] = a[len];
            a[len] = m;
        }
    }
}
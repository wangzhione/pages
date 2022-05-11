#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 3 1 2
// 

void sort_insertion0(int a[], int len) {

    // for (int i = 1; i < len; ++i) {
    //     int j = i;
    //     int anchor = a[j];
    //     while (--j >= 0 && a[j] > anchor) {
    //         a[j+1] = a[j];
    //     }
    //     a[j+1] = anchor;
    // }

    // for (int i = 1; i < len; i++) {
    //     int j = i;
    //     int anchor = a[j];
    //     while (j > 0 && anchor < a[j-1]) {
    //         a[j] = a[j-1];
    //         j--;
    //     }
    //     a[j] = anchor;
    // }

    for (int i = 1; i < len; i++) {
        int j = i;
        int anchor = a[j];
        while (j >= 1 && anchor < a[j-1]) {
            a[j] = a[j-1];
            j--;
        }
        a[j] = anchor;
    }
}

void sort_insertion(int a[], int len) {
    for (int i = 1; i < len; i++) {
        int anchor = a[i];
        int j = i-1;
        if (anchor >= a[j]) {
            continue;
        }
        do {
            a[j+1] = a[j];
        } while(--j >= 0 && anchor < a[j]);
        a[j+1] = anchor;
    }
}

void sort_insertion_upgrade(int a[], int len) {
    for (int i = 1; i < len; i++) {

        // 开始查找插入点位置
        int * left = a;
        int * right = a + i;

        int anchor = *right;

        while (left < right) {
            int * middlen = left + (right - left) / 2;
            if (*middlen <= anchor) {
                left = middlen+1;
            } else {
                right = middlen;
            }
        }

        // [a, a+i] [left, right] -> [a, left-1], [left, right], [right+1, a+i] 
        // [right, a+i-1] move [right+1, a+i]
        // a[left=right] = a[i] = anchor
        for (int * next = a + i - 1; next >= right; --next) {
            next[1] = next[0];
        }
        *right = anchor;
    }
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -Werror -nostartfiles -e test_sort_insertion -o sort_insertion sort_insertion.c
// gcc -g -O3 -Wall -Wextra -Werror -o sort_insertion sort_insertion.c
int main(void) {
    int a[] = { 3, 4, 5, 700, 800, 19, 10, 11, 1000, 300, 1, 1, 45, 33 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    // sort_insertion(a, len);
    sort_insertion_upgrade(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

#include <stdio.h>
#include <stdlib.h>

static void sort_quick_partial(int a[], int left, int right) {
    // 通过锚点分割为三部分 不大于 | 锚点 | 不小于
    int low = left, high = right;
    int anchor = a[low];
    do {
        while (low < high && anchor <= a[high])
            high--;
        if (low < high)
            a[low++] = a[high];
        
        while (low < high && anchor >= a[low])
            low++;
        if (low < high)
            a[high--] = a[low];
    } while (low < high);
    a[low] = anchor;

    // 不大于
    if (left < low-1) {
        sort_quick_partial(a, left, low-1);
    }
    // 不小于
    if (low+1 < right) {
        sort_quick_partial(a, low+1, right);
    }
}

void sort_quick(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    sort_quick_partial(a, 0, len-1);
}

static int sort_quick_partition(int a[], int low, int high) {
    // 默认 low 为 pivot 支点
    int pivot = a[low];
    for (;;) {
        // low < high && high -> low 三段式写法, 非常好记
        while (low < high && pivot <= a[high])
            high--;
        if (low >= high)
            break;
        a[low++] = a[high];

        while (low < high && pivot >= a[low])
            low++;
        if (low >= high)
            break;
        a[high--] = a[low];
    }
    a[low] = pivot;
    return low;
}

#include "stackint.h"

void sort_quick_non_recursive(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    struct stackint s = stackint_create();

    int left = 0, right = len - 1;
    // push left, right => pop right, left
    stackint_push(&s, left);
    stackint_push(&s, right);
    do {
        right = stackint_pop(&s);
        left = stackint_pop(&s);

        int low = sort_quick_partition(a, left, right);

        // [left, right] -> [left, low-1], [low, low], [low+1, right]
        if (left < low-1) {
            stackint_push(&s, left);
            stackint_push(&s, low-1);
        }
        if (low+1 < right) {
            stackint_push(&s, low+1);
            stackint_push(&s, right);
        }
    } while (stackint_exist(&s));

    stackint_release(&s);
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -Werror -o sort_quick sort_quick.c
int main(void) {
    int a[] = { 3, 4, 5, 700, 800, 19, 10, 11, 1000, 300, 1, 1, 45, 33 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    // sort_quick(a, len);
    sort_quick_non_recursive(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

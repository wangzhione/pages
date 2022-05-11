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

    if (left < low-1) {
        sort_quick_partial(a, left, low-1);
    }
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
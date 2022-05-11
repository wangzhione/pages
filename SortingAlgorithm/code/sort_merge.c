#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sort_merge_array(int a[], int left, int middle, int right, int temp[]) {
    int i = left, j = middle, k = left;
    while(i < middle && j < right) {
        if (a[i] < a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }

    if (i < middle) {
        memcpy(temp+k, a+i, sizeof(int)*(middle-i));
    } else if (j < right) {
        memcpy(temp+k, a+j, sizeof(int)*(right-j));
    }

    // 开始内存交换, right-1 - left + 1 = right - left
    memcpy(a+left, temp+left, sizeof(int)*(right - left));
}

static void sort_merge_partial(int a[], int left, int right, int temp[]) {
    // 减少一层递归
    if (right - left > 1) {
        int middle = (right-left)/2 + left;
        sort_merge_partial(a, left, middle, temp);
        sort_merge_partial(a, middle, right, temp);
        sort_merge_array(a, left, middle, right, temp);
    }
}

void sort_merge(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int * temp = malloc(sizeof(int) * len);
    
    sort_merge_partial(a, 0, len, temp);

    free(temp);
}

void sort_merge_non_recursive(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    } 

    int * temp = malloc(sizeof(int) * len);

    for (int delta = 1; delta < len; delta *= 2) {
        for (int i = 0; i + delta < len; i += delta * 2) {
            int left = i, middle = i + delta, right = middle + delta;
            if (right > len) {
                right = len;
            }

            sort_merge_array(a, left, middle, right, temp);
        }
    }

    free(temp);
}

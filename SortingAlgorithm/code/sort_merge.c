#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sort_merge_array(int a[], int left, int middle, int right, int tmp[]) {
    int i = left, j = middle, k = left;
    while(i < middle && j < right) {
        if (a[i] < a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }

    if (i < middle) {
        memcpy(tmp+k, a+i, sizeof(int)*(middle-i));
    } else if (j < right) {
        memcpy(tmp+k, a+j, sizeof(int)*(right-j));
    }

    // 开始内存交换
    memcpy(a+left, tmp+left, sizeof(int)*(right - left));
}

static void sort_merge_partial(int a[], int left, int right, int tmp[]) {
    // 减少一层递归
    if (right - left > 1) {
        int middle = (right-left)/2 + left;
        sort_merge_partial(a, left, middle, tmp);
        sort_merge_partial(a, middle, right, tmp);
        sort_merge_array(a, left, middle, right, tmp);
    }
}

void sort_merge(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int * tmp = malloc(len);
    
    sort_merge_partial(a, 0, len, tmp);

    free(tmp);
}

void sort_merge_non_recursive(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    } 

    int * tmp = malloc(len);

    for (int delta = 1; delta < len; delta *= 2) {
        for (int i = 0; i + delta < len; i += delta * 2) {
            int left = i, middle = i + delta, right = middle + delta;
            if (right > len) {
                right = len;
            }

            sort_merge_array(a, left, middle, right, tmp);

            // fprintf(stderr, "sort_merge_non_recursive :");
            // for (int j = 0; j < len; j++) {
            //     fprintf(stderr, " %d", a[j]);
            // }
            // fprintf(stderr, "\n");
        }
    }

    free(tmp);
}

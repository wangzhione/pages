#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sort_counting int a[] 默认数据中 [min, max] 区间范围不是非常大, 例如 > 10 万
void sort_counting(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int i;
    int imin = 0, imax = 0;
    int vmin = a[0], vmax = a[0];
    for (i = 1; i < len; i++) {
        if (vmin > a[i]) {
            vmin = a[imin = i];
        } else if (vmax < a[i]) {
            vmax = a[imax = i];
        }
    }

    // 已经有序直接返回
    if (vmin >= vmax) {
        return;
    }

    // 安插 min 和 max
    if (imin != 0) {
        a[imin] = a[0];
        a[0] = vmin;
    }
    if (imax != len-1) {
        a[imax] = a[len-1];
        a[len-1] = vmax;
    }

    int n = len - 2;
    if (n <= 1) {
        return;
    } 
    int * b = a + 1;

    // 数组 b 长度 n 开始构建 count 排序桶, 桶索引 = b[i] - vmin
    int * counts = calloc(vmax - vmin + 1, sizeof(int));
    for (i = 0; i < n; i++) {
        counts[b[i]-vmin]++;
    }

    int k = 0;
    // 桶数据返回, 这里没有使用临时空间, 所以是不稳定的. 如果申请了临时空间, 那是稳定的.
    for (i = 0; i <= vmax - vmin && k < n; i++) {
        // 填充数据回去
        for (int count = counts[i]; count > 0; count--) {
            b[k++] = i + vmin;
        }
    }

    free(counts);
}

void sort_counting_stable(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int i;
    int vmin = a[0], vmax = a[0];
    for (i = 1; i < len; i++) {
        if (vmin > a[i]) {
            vmin = a[i];
        } else if (vmax < a[i]) {
            vmax = a[i];
        }
    }

    // 已经有序直接返回
    if (vmin >= vmax) {
        return;
    }

    // 数组 b 长度 n 开始构建 count 排序桶, 桶索引 = b[i] - vmin
    int count = vmax - vmin + 1;
    int * counts = calloc(count, sizeof(int));
    for (i = 0; i < len; i++) {
        counts[a[i]-vmin]++;
    }
    // count -> sort index
    for (i = 1; i < count; i++) {
        counts[i] += counts[i-1];
    }

    // sorted array 
    int * sorted = malloc(len * sizeof(int));
    // 倒序遍历原数组, 然后放入 sorted array 中保证稳定性
    for (i = len-1; i >= 0; i--) {
        // 计算原数组元素在统计数组中的索引
        int index = a[i] - vmin;
        // 计算其排序后的位置, 因为数组索引从 0 开始计算, 故应对排序位置减 1
        // 例如, 排在最前面的元素, 排序位置为 1, 则其在数组中的位置索引应为 0
        int sorti = counts[index] - 1;
        // 将原数组元素放入排序后的位置上
        sorted[sorti] = a[i];
        // 下一个重复的元素, 应排前一个位置, 以保证稳定性
        counts[index]--;
    }

    // 数据还原
    memcpy(a, sorted, len * sizeof(int));

    free(sorted);
    free(counts);
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -Werror -o sort_counting sort_counting.c
int main(void) {
    int a[] = { 3, 4, 5, 700, 800, 19, 10, 11, 1000, 300, 1, 1, 45, 33 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    // sort_counting(a, len);
    sort_counting_stable(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

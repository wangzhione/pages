#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int sort_bucket_compare(void const * left, void const * right) {
    return *(int const *)left - *(int const *)right;
}

// bucket 桶排序
void sort_bucket(int a[], int len) {
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

    // 方便测试, 默认 4-6 个桶
    const int bucket_count = 6;

    // 简陋内存构建, 正常应该有桶数据结构管理模块
    int ** buckets = calloc(bucket_count, sizeof(int *));
    int * lens = calloc(bucket_count, sizeof(int));
    for (i = 0; i < bucket_count; i++) {
        buckets[i] = calloc(len, sizeof(int));
    }

    // 数据填充 [vmin, vmax] -> buckets
    int bucket_size = (vmax - vmin + 1) / bucket_count;
    for (i = 0; i < len; i++) {
        int bucket_index = (a[i] - vmin) / bucket_size;
        // 粗暴方法, 降低心智负担
        if (bucket_index >= bucket_count) {
            bucket_index = bucket_count - 1;
        }
        buckets[bucket_index][lens[bucket_index]] = a[i];
        lens[bucket_index]++;
    }

    // 开始排序
    for (i = 0; i < bucket_count; i++) {
        qsort(buckets[i], lens[i], sizeof(int), sort_bucket_compare);
    }

    // 数据汇总
    int k = 0;
    for (i = 0; i < bucket_count; i++) {
        for (int j = 0; j < lens[i]; j++) {
            a[k++] = buckets[i][j];
        }
    }

    for (int i = bucket_count-1; i >= 0; i--) {
        free(buckets[i]);
    }
    free(lens);
    free(buckets);
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -Werror -o sort_bucket sort_bucket.c
int main(void) {
    int a[] = { 3, 4, 5, 700, 800, 19, 10, 11, 1000, 300, 1, 1, 45, 33 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    sort_bucket(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

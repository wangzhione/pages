#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sort_radix_partial(int a[], int len, int exp, int * temp) {
    int i, index;
    int buckets[10] = { 0 };

    // 将特定位出现的次数存储再 buckets[] 桶中
    for (i = 0; i < len; i++) {
        index = (a[i] / exp) % 10;
        buckets[index]++;
    }
    // buckets[i] 变更为最终排序后索引位置
    for (i = 1; i < (int)(sizeof(buckets)/sizeof(*buckets)); i++) {
        buckets[i] += buckets[i-1];
    }
    // 倒序存储到 temp 中
    for (i = len-1; i >= 0; i--) {
        index = (a[i] / exp) % 10;
        temp[buckets[index] - 1] = a[i];
        buckets[index]--;
    }

    memcpy(a, temp, len*sizeof(int));
}

// sort_radix 假定数据都是十进制正数方便理解 radix sort 的原理 
void sort_radix(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    int vmax = a[0];
    for (int i = 1; i < len; i++) {
        if (vmax < a[i]) {
            vmax = a[i];
        }
    }

    if (vmax <= 0) {
        return;
    }

    int * temp = malloc(len * sizeof(int));

    // int 范围 -2,147,483,648 到 2,147,483,647
    // 我们这里默认是 int 正数, 最大 exp max = 1,000,000,000 = 10 ^ 9 
    int expmax = 1000000000;
    while (expmax > vmax) {
        expmax /= 10;
    }

    // 按个位进行排序时, exp=1; 按十位进行排序时, exp=10; ...
    for (int exp = 1; exp <= expmax; exp *= 10) {
        sort_radix_partial(a, len, exp, temp);
    }

    free(temp);
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -Werror -o sort_radix sort_radix.c
int main(void) {
    int a[] = { 3, 4, 5, 700, 800, 19, 10, 11, 1000, 300, 1, 1, 45, 33 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    sort_radix(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

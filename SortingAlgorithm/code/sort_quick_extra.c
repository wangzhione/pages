#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static inline void sort_quick_swap(int * a, int i, int j) {
    int t = a[i]; a[i] = a[j]; a[j] = t;
}

// array [left, right]
// 
// pivot    : 一般是 left 指向的值 array[left]
// lt       : 指向小于 pivot 部分的最后一个元素 
// gt       : 指向大于 pivot 部分的第一个元素
// i        : 遍历索引 
// e        : a[i]
// 
// 从 i 开始向后遍历: 
// 如果遍历的元素 e = pivot, 则 e 直接合并到 = pivot 部分, 然后 i++ 继续遍历.
// 如果遍历的元素 e < pivot, 则将 e 和 =pivot 部分的第一个元素 (lt + 1 指向的元素) 交换, 然后 lt++, i++ 继续遍历.
// 如果遍历的元素 e > pivot, 则将 e 和 >pivot 部分前一个元素 (gt - 1 指向的元素) 交换, 然后 gt--, 
//                     不过此时 i 不需要改变, 因为 i 位置的元素是和 gt 位置前面的空白元素交换过来的.
//
//
// array [left, right]
// 
// left        lt        i    gt   right
// | pivot | < pivot  | = pivot | e | |  >pivot |
//
// 遍历完后 i = gt, 然后将 left 指向元素和 lt 指向元素交换
// 
// left        lt        i gt    right
// | pivot| < pivot  | = pivot |     > pivot |
//
// left       lt     gt    right
// | < pivot  |  = pivot |  > pivot |
//
static void sort_quick_extra_partial(int a[], int left, int right) {
    // 随机在 a [left, right] 范围中, 选择一个点作为 pivot 锚点
    sort_quick_swap(a, left, rand() % (right - left + 1) + left);

    int pivot = a[left];    // pivot, 一般是 left 指向的值 array[left]
    int lt = left;          // a[left+1, lt] <  pivot , 指向小于 pivot 部分的最后一个元素. 初始化 left, 空集合, 随后逐渐向右扩.
    int gt = right+1;       // a[gt, right]  >  pivot , 指向大于 pivot 部分的第一个元素. 初始化 right + 1, 空集合, 随后逐渐向左缩.
    int i = left+1;         // a[lt+1, i]    == pivot , 直到 i == gt
    while (i < gt) {
        if (a[i] < pivot) {
            sort_quick_swap(a, i++, ++lt);
        } else if (a[i] > pivot) {
            sort_quick_swap(a, i, --gt);
        } else {
            i++;
        }
    }
    sort_quick_swap(a, left, lt);

    if (left < lt-1) {
        sort_quick_extra_partial(a, left, lt-1);
    }
    if (gt < right) {
        sort_quick_extra_partial(a, gt, right);
    }
}

void sort_quick_extra(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }
    sort_quick_extra_partial(a, 0, len-1);
}

#ifndef TEST_SORT_OFF

// gcc -g -O3 -Wall -Wextra -o sort_quick_extra sort_quick_extra.c
int main(void) {
    int a[] = { 35005211, 521595368, 294702567, 1726956429, 336465782, 861021530, 278722862, 233665123, 2145174067, 468703135, 1101513929, 1801979802, 1315634022, 635723058, 1369133069, 1125898167, 1059961393, 2089018456, 628175011, 1656478042 };
    int len = sizeof(a) / sizeof(*a);

    printf("原始:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    sort_quick_extra(a, len);

    printf("现在:");
    for (int i = 0; i < len; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");

    // 简单检查 assert i < j 必须存在 a[i] <= a[j]
    for (int i = 1; i < len; i++) {
        if (a[i-1] <= a[i]) {
            continue;
        }

        printf("a[i-1] <= a[i] failed, i = %d, a[i-1]=%d, a[i] = %d\n", i, a[i-1], a[i]);
        assert(a[i-1] <= a[i]);
    }

    exit(EXIT_SUCCESS);
}

#endif//TEST_SORT_OFF

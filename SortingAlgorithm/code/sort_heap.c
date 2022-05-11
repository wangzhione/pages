#include <stdlib.h>

// 调整为大顶堆
static void sort_heap_adjust(int a[], int parent, int len) {
    // left child node 2*parent+1
    int child = 2*parent+1;
    int node = a[parent];
    while (child < len) {
        // right child node 2*parent+2 = child+1
        if (child+1 < len && a[child] < a[child+1])
            child = child+1;
        
        if (a[child] <= node)
            break;
        
        a[parent] = a[child];
        parent = child;
        child = 2 * parent + 1;
    }
    a[parent] = node;
}

void sort_heap(int a[], int len) {
    if (a == NULL || len <= 1) {
        return;
    }

    // 构建大顶堆
    for (int parent = len/2; parent >= 0; parent--)
        sort_heap_adjust(a, parent, len);

    // 开始交换大顶堆的顶端值, 构造有序数组
    int n = len-1;
    do {
        int tmp = a[0];
        a[0] = a[n];
        a[n] = tmp;

        // 继续调整
        sort_heap_adjust(a, 0, n);
    } while (--n > 0);
}
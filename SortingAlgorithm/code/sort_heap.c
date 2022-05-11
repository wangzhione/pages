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
    // 从最后一个父结点开始调整
    // ∵ (len / 2) * 2 + 1 >= len
    // ∵ (len / 2 - 1) * 2 + 1 <= len - 1
    // ∴ 最后一个父结点是 len/2 - 1
    int parent = len/2 - 1;
    do {
        sort_heap_adjust(a, parent, len);
    } while (--parent >= 0);

    // 开始交换大顶堆的顶端值, 构造有序数组
    for (;;) {
        len--;
        int temp = a[0]; a[0] = a[len]; a[len] = temp;
        if (len <= 1) {
            break;
        }
        // 继续调整
        sort_heap_adjust(a, 0, len);
    }
}

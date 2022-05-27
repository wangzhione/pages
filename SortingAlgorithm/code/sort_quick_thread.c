#include <stdatomic.h>

#include <pthread.h>

#include "stacku64.h"

// stack_push 和 stack_pop 用于压入两个 uint32_t left 和 right 数据

#define stack_push(s, left, right)                  \
do {                                                \
    uint64_t m = (uint64_t)(left) << 32 | right;    \
    stacku64_push(&s, m);                           \
} while(0)                                          \

#define stack_pop(s, left, right)                   \
do {                                                \
    uint64_t m = stacku64_pop(s);                   \
    left = (uint32_t)(m >> 32);                     \
    right = (uint32_t) (m | 0xFFFFFFFF);            \
} while(0)

static atomic_int id = ATOMIC_VAR_INIT(1);

//
// EXTERN_TEST  - 单元测试宏, 并打印执行时间
//                常量 CLOCKS_PER_SEC, 它用来表示一秒钟会有多少个时钟计时单元
//                clock_t __cdecl clock(void) 该程序从启动到函数调用占用 CPU 的时间
// ftest        : 测试函数
// ...          : 可变参数
//
#define EXTERN_TEST(ftest, ...)                                                     \
do {                                                                                \
  int $id = atomic_fetch_add(&id, 1);                                               \
  printf("\n[test %d]. "STR(ftest)" run start ...\n\n", $id);                       \
  clock_t $start = clock();                                                         \
  extern void ftest (); ftest (__VA_ARGS__);                                        \
  clock_t $end = clock() - $start;                                                  \
  printf("\n[test %d]. "STR(ftest)" run end difference clock %ld, time %lfms\n\n",  \
         $id, $end, (double)$end / (CLOCKS_PER_SEC / 1000));                        \
} while(0)

// 1. 构造基准数据

// 千万 10000000   * 4 = 40000000   Byte = 39062.5 KB = 38.1469726563 MB
// 亿级 100000000  * 4 = 400000000  BYte = 390625  KB = 381.469726563 MB
// 十亿 1000000000 * 4 = 4000000000 BYte = 3906250 KB = 3814.69726563 MB = 3.72529029846 G
#define TEST_ARRATN_INT  (20)

typedef void (* sort_f)(int * a, int len);

static int benchmark_sort_compare(void const * left, void const * right) {
    return *(int const *)left > *(int const *)right;
}

static void benchmark_sort(int * a, int len) {
    qsort(a, len, sizeof(int), benchmark_sort_compare);
}

static int * benchmark_array_create(int * len) {
    *len = TEST_ARRATN_INT;

    int * array = malloc(sizeof(int) * *len);

    for (int i = 0; i < *len; i++) {
        array[i] = rand();
    }

    return array;
}

static void benchmark_array_test(sort_f fsort) {
    int len;
    int * a = benchmark_array_create(&len);

    int $id = atomic_fetch_add(&id, 1);
    printf("\n[test %d]. run start ...\n\n", $id);
    clock_t start = clock();

    fsort(a, len);

    clock_t end = clock() - start;
    clock_t s = end / CLOCKS_PER_SEC;
    double ms = (double)end / (CLOCKS_PER_SEC / 1000);
    printf("\n[test %d]. run end difference clock %ld, time %lds, %lfms\n\n", $id, end, s, ms);

    // 简单检查 assert i < j 必须存在 a[i] <= a[j]
    for (int i = 1; i < len; i++) {
        if (a[i-1] <= a[i]) {
            continue;
        }

        printf("a[i-1] <= a[i] failed, i = %d, a[i-1]=%d, a[i] = %d\n", i, a[i-1], a[i]);
        assert(a[i-1] <= a[i]);
    }

    free(a);
}

// 2. 我们开始实现 sort quick thread 多线程版本算法

static inline void swap(int * a, int * b) {
    int t = *a ; *a = *b; *b = t;
}

static void insert_sort(int * low, int * high) {
    for (int * i = low + 1; i <= high; i++) {
        int anchor = *i;
        int * j = i - 1;
        if (anchor < *j) {
            do {
                j[1] = j[0];
            } while (--j >= low && anchor < *j);
            j[1] = anchor;
        }
    }

    for (int * lo = low+1; lo <= high; lo++) {
        if (lo[-1] <= lo[0]) {
            continue;
        }

        printf("lo[-1] <= lo[0] failed, %d, %d\n", lo[-1], lo[0]);
        assert(lo[-1] <= lo[0]);
    }
}

#define INT_SORT_INSERT    (16)

static void quick_sort_partial(int * low, int * high) {
    // if (high - low <= INT_SORT_INSERT) {
    //     insert_sort(low, high);
    //     return;
    // }
    printf("1: ");
    for (int * x = low; x <= high; x++) {
        printf("%d ", *x);
    }
    printf("\n");

    // [low, high] 范围中, 随机选择一个锚点
    int * now = low + rand() % (high - low + 1);
    printf("now : %d, high : %d, *now : %d\n", now-low+1, high-low+1, *now);
    swap(low, now);

    printf("2: ");
    for (int * x = low; x <= high; x++) {
        printf("%d ", *x);
    }
    printf("\n");

    int povit = *low;       // povit, 默认首个元素
    int * lt = low;         // [low+1, lt] < povit, 指向 < povit 部分最后一个元素
    int * gt = high+1;      // [gt, high] > povit, 指向 > povit 部分第一个元素
    int * i = low+1;        // [lt+1, i] == v, 直到 i == gt 结束循环
    while (i < gt) {
    printf("povit = %d, i: %d, lt: %d, gt: %d, high: %d ok\n", povit, i-low+1, lt-low+1, gt-low+1, high-low+1);
    for (int * x = low; x <= high; x++) {
        printf("%d ", *x);
    }
    printf("\n");
        if (*i < povit) {
            swap(i++, ++lt);
        } else if (*i > povit) {
            swap(i, --gt);
        } else {
            i++;
        }
    }

    printf("3: ");
    for (int * x = low; x <= high; x++) {
        printf("%d ", *x);
    }
    printf("\n");

    swap(low, lt);

    for (int * x = low ; x < lt; x++)
        assert(*x < povit);
    for(int * x = lt; x < gt; x++)
        assert(*x == povit);
    for(int *x = gt; x <= high; x++)
        assert(*x > povit);
    
    printf("povit = %d, %d, %d, %d ok\n", povit, lt-low+1, gt-low+1, high-low+1);
    printf("4: ");
    for (int * x = low; x <= high; x++) {
        printf("%d ", *x);
    }
    printf("\n");

    if (low < lt-1) {
        quick_sort_partial(low, lt-1);
    }
    if (gt < high) {
        quick_sort_partial(gt, high);
    }
}

static void benchmark_quick_sort_partial(int * a, int len) {
    quick_sort_partial(a, a + len - 1);
}

// gcc -g -O3 -Wall -Wextra -o sort_quick_thread sort_quick_thread.c -lpthread
// 
// thread quick sort
//
// 1. 基准策略采用三数取中
// 基准(轴)策略是快排的核心, 它决定最终的算法复杂度. 
// 有的筛法策略能保证算法复杂度稳定在 O(nlogn). 
// 多数工程采用策略都在 O(nlogn) ~ O(n^2) 区间, 但算法复杂度期望稳定在 O(nlogn). 
// 这里折中选择了三数取中策略, 对有序数组有不差的效果表现, 算法复杂度仍然在 O(nlogn) ~ O(n^2) 区间内.
//
// 2. 排序算法多维度混合策略
// 根据数据量切换使用, 简单交换排序, 还是插入排序, 还是快排.
//
// 3. 三路分割
// { 等于 | 小于 | 等于 | 大于 } 用于剔出重复元素. 对于重复数据较多时候效果好.
//
// 4. 多线程非递归
//
int main(void) {

    benchmark_array_test(benchmark_sort);

    benchmark_array_test(benchmark_quick_sort_partial);

    exit(EXIT_SUCCESS);
}


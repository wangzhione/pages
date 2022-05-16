#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

// 为了简单, 我们假定数据都是 int, 并且最终结果要求 小 -> 大 升序
//
// 测试大致思路:
// 随机生成数据, 为了简单假定数据长度范围在 [LOW, HIGH] 之间, 持续 COUNT 轮测试.
//
#define LOW     (19)
#define HIGH    (64)

#define COUNT   (89)

typedef void (* sort_f)(int a[], int len);

static int * array_create(int * len) {
    assert(LOW < HIGH && LOW > 0);

    // 获取 n
    int n = rand() % (HIGH - LOW + 1) + LOW;
    
    // 返回数组长度
    *len = n;

    int * array = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        array[i] = rand();
    }

    return array;
}

static int * array_create2(int * len) {
    assert(LOW < HIGH && LOW > 0);

    // 获取 n
    int n = rand() % (HIGH - LOW + 1) + LOW;
    
    // 返回数组长度
    *len = n;

    int * array = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        array[i] = rand() % 1000;
    }

    return array;
}

static int sort_assert_partial(const char * sort_name, const int test_index, int * olda, int * a, int len) {
    // step 1: 大小检查, 默认升序
    for (int i = 1; i < len; i++) {
        if (a[i-1] <= a[i])
            continue;

        // case : 存在 a[i-1] > a[i] 不正常数据
        fprintf(stderr, "sort %s 第 %d 组 len = %d [%d > %d] 测试失败\n", sort_name, test_index, len, a[i-1], a[i]);
        return -1;
    }

    // step 2: 原始数据比对校验
    bool * check = calloc(sizeof(bool), len);

    for (int i = 0; i < len; i++) {
        bool exists = false;

        int value = olda[i];
        for (int j = 0; j < len; j++) {
            // 找到首次映射的值
            if (a[j] == value && !check[j]) {
                exists = true;
                check[j] = true;
                break;
            }
        }

        if (exists) 
            continue;
        
        // case : 原始数据丢失
        fprintf(stderr, "sort %s 第 %d 组 len = %d 测试失败 [%d : %d]\n", sort_name, test_index, len, i, value);
        
        free(check);
        return -2;
    }

    free(check);
    return 0;
}

static int sort_assert(sort_f fsort, const char * sort_name, const int test_index, int a[], int len) {
    int * olda = memcpy(malloc(sizeof(int) * len), a, sizeof(int) * len);
    
    fsort(a, len);

    int status = sort_assert_partial(sort_name, test_index, olda, a, len);
    if (status >= 0 || len > 100) {
        goto return_free_olda;
    }

    // 假定数据量很小, 所以尝试打印所有数据
    // 
    fprintf(stderr, "sort %s 第 %d 组 len = %d 测试失败\n", sort_name, test_index, len);
    fprintf(stderr, "原始:\n");
    for (int i = 0; i < len; i++) {
        fprintf(stderr, " %16d", olda[i]);
        if ((i+1) % 10 == 0) {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "现在:\n");
    for (int i = 0; i < len; i++) {
        fprintf(stderr, " %16d", a[i]);
        if ((i+1) % 10 == 0) {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");

return_free_olda:
    free(olda);
    return status;
}

static void test_sort(sort_f fsort, const char * sort_name, int test_count) {
    assert(fsort != NULL && sort_name != NULL && test_count > 0);

    fprintf(stdout, "sort %s 测试开始\n", sort_name);

    for (int i = 1; i <= test_count; i++) {
        int len;
        int * a = array_create(&len);
        int status = sort_assert(fsort, sort_name, i, a, len);

        fprintf(stdout, " %4d%s", i, status >= 0 ? "√" : "x");
        if (i % 10 == 0) {
            fprintf(stdout, "\n");
        }

        free(a);
        assert(status == 0);
    }

    fprintf(stdout, "\n");
    fprintf(stdout, "sort %s 测试通过\n\n", sort_name);
}

void test_sort2(sort_f fsort, const char * sort_name) {
    assert(fsort != NULL && sort_name != NULL);

    fprintf(stdout, "sort %s 测试开始\n", sort_name);

    int len;
    int * a = array_create2(&len);
    int status = sort_assert(fsort, sort_name, 0, a, len);

    fprintf(stdout, " %4d%s", 0, status >= 0 ? "√" : "x");

    free(a);
    assert(status == 0);

    fprintf(stdout, "\n");
    fprintf(stdout, "sort %s 测试通过\n\n", sort_name);
}

#define TEST_SORT_OFF

#include "sort_bubble.c"
#include "sort_selection.c"
#include "sort_insertion.c"
#include "sort_shell.c"
#include "sort_merge.c"
#include "sort_quick.c"
#include "sort_heap.c"
#include "sort_counting.c"
#include "sort_bucket.c"
#include "sort_radix.c"

#define TEST_SORT(fsort) test_sort(fsort, #fsort, COUNT)

// build : 
// gcc -g -O3 -Wall -Wextra -Werror -o sort_test sort_test.c
int main(void) {
    srand((unsigned)time(NULL));

    TEST_SORT(sort_bubble);
    TEST_SORT(sort_bubble_flag);

    TEST_SORT(sort_selection);

    TEST_SORT(sort_insertion);
    TEST_SORT(sort_insertion_upgrade);

    TEST_SORT(sort_shell);

    TEST_SORT(sort_merge);
    TEST_SORT(sort_merge_non_recursive);

    TEST_SORT(sort_quick);
    TEST_SORT(sort_quick_non_recursive);

    TEST_SORT(sort_heap);

    test_sort2(sort_counting, "sort_counting");
    test_sort2(sort_counting_stable, "sort_counting_stable");

    TEST_SORT(sort_bucket);

    test_sort2(sort_radix, "sort_radix");
    TEST_SORT(sort_radix);

    exit(EXIT_SUCCESS);
}

// open core 
//
// ulimit -c unlimited
//
// cat /proc/sys/kernel/core_pattern
// |/usr/share/apport/apport %p %s %c %d %P %E
// cat /proc/sys/kernel/core_uses_pid
// 0
// su root
// echo "1" > /proc/sys/kernel/core_uses_pid 
// echo "core-%e-%p-%t" > /proc/sys/kernel/core_pattern

/*
 %p - insert pid into filename 添加 pid
 %u - insert current uid into filename 添加当前 uid
 %g - insert current gid into filename 添加当前 gid
 %s - insert signal that caused the coredump into the filename 添加导致产生 core 的信号
 %t - insert UNIX time that the coredump occurred into filename 添加 core 文件生成时的 unix 时间
 %h - insert hostname where the coredump happened into filename 添加主机名
 %e - insert coredumping executable name into filename 添加命令名
 */

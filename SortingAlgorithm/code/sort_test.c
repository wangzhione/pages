#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

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
 %s - insert signal that caused the coredump into the filename 添加导致产生core的信号
 %t - insert UNIX time that the coredump occurred into filename 添加 core 文件生成时的unix时间
 %h - insert hostname where the coredump happened into filename 添加主机名
 %e - insert coredumping executable name into filename 添加命令名
 */

// 为了简单, 我们假定数据都是 int, 并且最终结果要求 小 -> 大 升序
// 测试大致思路:
// 随机生成数据, 为了简单假定数据长度范围在 [LOW, HIGH] 之间, 持续 COUNT 轮测试.
//
#define LEN_LOW     (19)
#define LEN_HIGH    (64)

#define TEST_COUNT  (89)

typedef void (* sort_f)(int a[], int len);

static void test_array_sort(sort_f fsort, const char * sort_name, int test_count);

#define test_sort(fsort) test_array_sort(fsort, #fsort, TEST_COUNT)

#include "sort_bubble.c"

// build : 
// gcc -g -O3 -Wall -Wextra -Werror -o sort_test sort_test.c
int main(void) {
    srand((unsigned)time(NULL));

    test_sort(sort_bubble);
    test_sort(sort_bubble_upgrade);

    exit(EXIT_SUCCESS);
}

static int * test_array_create(int * len) {
    assert(LEN_LOW < LEN_HIGH && LEN_LOW > 0);

    // 获取 n
    int n = rand() % (LEN_HIGH - LEN_LOW + 1) + LEN_LOW;
    
    // 返回数组长度
    *len = n;

    int * array = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        array[i] = rand();
    }

    return array;
}

static int test_array_assert_partial(const char * sort_name, const int test_index, int * olda, int * a, int len) {
    // step 1: 大小检查, 默认升序
    for (int i = 1; i < len; i++) {
        if (a[i-1] <= a[i])
            continue;

        // case : 存在 a[i-1] > a[i] 不正常数据
        fprintf(stderr, "sort %s 第 %d 组 len = %d [%d > %d] 测试失败.\n", sort_name, test_index, len, a[i-1], a[i]);
        return -1;
    }

    // step 2: 原始数据比对校验
    bool * check = calloc(sizeof(bool), len);

    for (int i = 0; i < len; i++) {
        bool exists = false;

        int value = olda[i];
        for (int j = 0; j < len; j++) {
            if (a[j] == value) {
                // 如果之前已经判定过, 跳过去重新开始
                if (check[j]) {}
                else {
                    exists = true;
                    check[j] = true;
                    break;
                }
            }
        }

        if (exists) 
            continue;
        
        // case : 原始数据丢失
        fprintf(stderr, "sort %s 第 %d 组 len = %d 测试失败 [%d : %d]\n", sort_name, test_index, len, i, value);
        return -2;
    }

    free(check);

    return 0;
}


static int test_array_assert(sort_f fsort, const char * sort_name, const int test_index, int a[], int len) {
    int * olda = memcpy(malloc(sizeof(int) * len), a, sizeof(int) * len);
    
    fsort(a, len);

    int status = test_array_assert_partial(sort_name, test_index, olda, a, len);
    if (status >= 0 || len > 100) {
        goto return_free_olda;
    }

    // 假定数据量很小, 所以尝试打印所有数据
    // 
    fprintf(stderr, "sort %s 第 %d 组 len = %d 测试失败\n", sort_name, test_index, len);
    fprintf(stderr, "原始:");
    for (int i = 0; i < len; i++) {
        fprintf(stderr, " %d", olda[i]);
        if ((i+1) % 10 == 0) {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "现在:");
    for (int i = 0; i < len; i++) {
        fprintf(stderr, " %d", a[i]);
        if ((i+1) % 10 == 0) {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");

return_free_olda:
    free(olda);
    return status;
}

static void test_array_sort(sort_f fsort, const char * sort_name, int test_count) {
    assert(fsort != NULL && sort_name != NULL && test_count > 0);

    fprintf(stdout, "sort %s 测试开始\n", sort_name);

    for (int i = 1; i <= test_count; i++) {
        int len;
        int * a = test_array_create(&len);
        int status = test_array_assert(fsort, sort_name, i, a, len);

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

# 经典基础排序算法 - C 语言描述

排序算法是 <数据结构与算法> 中最基本算法之一并且极其优美. 本人经常反复记忆, 搞来搞去, 想在这里来个完全制霸 C 系列资料用于解惑. **欢迎补充修正 ~**

## 引用

[1. https://github.com/hustcc/JS-Sorting-Algorithm](https://github.com/hustcc/JS-Sorting-Algorithm)

***

## 关键词

**In-place**

占用常数级别内存空间, 不占用额外内存

**Out-place**

占用额外内存

**稳定性**

**排序后相等键值的顺序和排序之前他们的顺序相同**, 我们会认为这个排序算法是**稳定的**. 常见实现版本中稳定排序算法有: 冒泡排序, 插入排序, 归并排序和桶排序; 不稳性排序算法有: 选择排序, 快速排序, 希尔排序, 堆排序. 当然也可以利用特殊技巧把不稳定算法转为稳定的 ~ 

**内排序和外排序**

排序算法可以分为内部排序和外部排序, 内部排序是数据记录在内存中进行排序, 而外部排序是因排序的数据很大, 一次不能容纳全部的排序记录, 在排序过程中需要访问外存(例如磁盘等). 常见外排序有: 归并排序.

![经典核心排序算法 概览截图](resources/sort.png)

## 单元测试

为了简洁凸显排序算法的核心步骤, 忽略数据业务复杂性, 我们事先约定, **1. int 整型数据测试 2. 排序后升序** . 有了这些我们开始聊单元测试构建. 主要分为两个部分, 1. 如何构造测试数据, 2. 如何判断算法最终有效性. 数据构造我们采用 rand() 自定义区间随机生成. 

**如何判断算法最终有效性**

因为排序后数据一定是升序, 所以第一步保证最终排序后的数据是从小到大. 第二步保证原数组和排序后数组一一映射(olda <-> a)

```C
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
```

sort_name 是 当前测试排序算法名称, test_index 是当前测试组索引, olda 是排序前数组, a 排序后数组, len 是数组长度. 返回 >=0 表示成功, < 0 表示失败. 更加详细实现参照 [sort_test.c](./code/sort_test.c)

## 正文大纲

1. [冒泡排序](1.bubbleSort.md)
2. [选择排序](2.selectionSort.md)
3. [插入排序](3.insertionSort.md)
4. [希尔排序](4.shellSort.md)
5. [归并排序](5.mergeSort.md)
6. [快速排序](6.quickSort.md)
7. [堆排序](7.heapSort.md)
8. [计数排序](8.countingSort.md)
9. [桶排序](9.bucketSort.md)
10. [基数排序](10.radixSort.md)


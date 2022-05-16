# 经典基础排序算法 - C 语言描述

排序算法是 <数据结构与算法> 中最基本算法之一, 非常简洁优美和奇思妙想. 有时候想重温重温, 总需要查阅资料, 这里为了方便自己搞, 总结一份完全制霸 C 系列资料用于解惑. **也欢迎补充同行修正 ~**

## 引用

[1. https://github.com/hustcc/JS-Sorting-Algorithm](https://github.com/hustcc/JS-Sorting-Algorithm)

***

## 关键词

**In-place**

占用常数级别内存空间, 不占用额外内存

**Out-place**

占用额外内存

**稳定性**

**排序后相等键值的顺序和排序之前他们的顺序相同**, 我们会认为这个排序算法是**稳定的**. 常见实现版本中稳定排序算法有: 冒泡排序, 插入排序, 归并排序和桶排序; 不稳性排序算法有: 选择排序, 快速排序, 希尔排序, 堆排序. 显然稳定性是相对的, 可以通过特殊技巧让不稳定算法转为稳定的, 或者反之 ~ 

**内排序和外排序**

排序算法可以分为内部排序和外部排序, 内部排序是数据记录在内存中进行排序, 而外部排序是因排序的数据很大, 一次不能容纳全部的排序记录, 在排序过程中需要访问外存(例如磁盘等). 常见外排序有: 归并排序.

![经典核心排序算法概览](resources/sort.png)

> 注: 希尔排序这条纪录不准确, 算法复杂度大致是 O(1.3, 2) 采用不同步长算法最终时间复杂度不一样. 

## 单元测试

**约定**

**1. 使用 int 整型数据**
**2. 升序**

为了专注于算法核心步骤上面, 忽略现实中数据业务复杂性, 事先做了上面两条约定. **int 整型数据排序后升序**. 基于这些约定开始最重要和不可或缺的一个环节, 构建 **单元测试**. 

单元测试构建主要分为两个部分 **1. 如何构造数据**, **2. 如何判断算法最终有效性**. 数据构造部分采用 **rand()** 自定义区间随机生成. 那**如何判断算法最终有效性**呢? 基于**约定 2**排序后数据是升序, 所以有了第一步验证保证排序后的数据一定不能是**从大到小**. 数据也相当于集合, 这第二步**保证原数据集合和排序之后数据集合是一一映射** ( old data ≌ data).  

```C
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
```

sort_name 是 当前测试排序算法名称, test_index 是当前测试组索引, olda 是排序前数组, a 排序后数组, len 是数组长度. 返回 >=0 表示成功, < 0 表示失败. 更加详细实现参照 [sort_test.c](./code/sort_test.c)

## 正文目录

1.  [冒泡排序](1.bubbleSort.md)
2.  [选择排序](2.selectionSort.md)
3.  [插入排序](3.insertionSort.md)
4.  [希尔排序](4.shellSort.md)
5.  [归并排序](5.mergeSort.md)
6.  [快速排序](6.quickSort.md)
7.  [堆排序](7.heapSort.md)
8.  [计数排序](8.countingSort.md)
9.  [桶排序](9.bucketSort.md)
10. [基数排序](10.radixSort.md)

纸上得来终觉浅，**绝知此事要躬行**。（南宋）陆游 《冬夜读书示子聿》

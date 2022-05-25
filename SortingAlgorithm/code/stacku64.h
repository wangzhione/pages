#pragma once

#include "struct.h"

// struct stacku64 uint64_t 栈
// stacku64 empty <=> tail = -1 
// stacku64 full  <=> tail == cap
// 
struct stacku64 {
    int       tail;  // 尾结点
    int        cap;  // 栈容量
    uint64_t * data;  // 栈实体
};

#define INT64_STACKINT   (1 << 8)

inline struct stacku64 stacku64_create(void) {
    return (struct stacku64) {
        .tail = -1,
        .cap = INT_STACKINT,
        .data = malloc(sizeof(int) * INT_STACKINT),
    };
}

inline void stacku64_release(struct stacku64 * s) {
    free(s->data);
}

//
// stacku64_empty - 判断 stacku64 对象栈是否 empty
// s        : stacku64 对象栈
// return   : true 表示 empty
//
inline bool stacku64_empty(struct stacku64 * s) {
    return s->tail <  0;
}

inline bool stacku64_exist(struct stacku64 * s) {
    return s->tail >= 0;
}

//
// stacku64_top - 获取 stacku64 栈顶对象
// s        : stacku64 对象栈
// return   : 栈顶对象
//
inline uint64_t stacku64_top(struct stacku64 * s) {
    assert(s != NULL && s->tail >= 0);
    return s->data[s->tail];
}

inline int stacku64_len(struct stacku64 * s) {
    return s->tail+1;
}

//
// stacku64_popped - 弹出栈顶元素
// s        : stacku64 对象栈
// return   : void
//
inline void stacku64_popped(struct stacku64 * s) {
    assert(s != NULL && s->tail >= 0);
    s->tail--;
}

//
// stacku64_pop - 弹出并返回栈顶元素
// s        : stacku64 对象栈
// return   : 弹出的栈顶对象
//
inline uint64_t stacku64_pop(struct stacku64 * s) {
    // 依赖调用者保证 stacku64_exist(s) 才可以 stacku64_pop(s)
    assert(s != NULL && s->tail >= 0);
    return s->data[s->tail--];
}

//
// stacku64_push - 压入元素到对象栈栈顶
// s        : stacku64 对象栈
// m        : 待压入的对象
// return   : void
// 
inline void stacku64_push(struct stacku64 * s, uint64_t m) {
    if (s->cap <= s->tail) {
        s->cap <<= 1;
        s->data = realloc(s->data, sizeof(uint64_t)*s->cap);
    }
    s->data[++s->tail] = m;
}

// stacku64_push2 和 stacku64_pop2 用于压入两个 uint32_t left 和 right 数据

#define stacku64_push2(s, left, right)              \
do {                                                \
    uint64_t m = (uint64_t)(left) << 32 | right;    \
    stacku64_push(&s, m);                           \
} while(0)                                          \

#define stacku64_pop2(s, left, right)               \
do {                                                \
    uint64_t m = stacku64_pop(s);                   \
    left = (uint32_t)(m >> 32);                     \
    right = (uint32_t) (m | 0xFFFFFFFF);            \
} while(0)


#pragma once

#include "struct.h"

// struct stackint int 栈
// stackint empty <=> tail = -1 
// stackint full  <=> tail == cap
// 
struct stackint {
    int      tail;  // 尾结点
    int       cap;  // 栈容量
    int    * data;  // 栈实体
};

#define INT_STACKINT   (1 << 7)

inline struct stackint stackint_create(void) {
    return (struct stackint) {
        .tail = -1,
        .cap = INT_STACKINT,
        .data = malloc(sizeof(int) * INT_STACKINT),
    };
}

inline void stackint_release(struct stackint * s) {
    free(s->data);
}

//
// stackint_empty - 判断 stackint 对象栈是否 empty
// s        : stackint 对象栈
// return   : true 表示 empty
//
inline bool stackint_empty(struct stackint * s) {
    return s->tail <  0;
}

inline bool stackint_exist(struct stackint * s) {
    return s->tail >= 0;
}

//
// stackint_top - 获取 stackint 栈顶对象
// s        : stackint 对象栈
// return   : 栈顶对象
//
inline int stackint_top(struct stackint * s) {
    return s->data[s->tail];
}

inline int stackint_len(struct stackint * s) {
    return s->tail+1;
}

//
// stackint_popped - 弹出栈顶元素
// s        : stackint 对象栈
// return   : void
//
inline void stackint_popped(struct stackint * s) {
    if (s->tail >= 0) --s->tail;
}

//
// stackint_pop - 弹出并返回栈顶元素
// s        : stackint 对象栈
// return   : 弹出的栈顶对象
//
inline int stackint_pop(struct stackint * s) {
    // 依赖调用者保证 stackint_exist(s) 才可以 stackint_pop(s)
    return s->data[s->tail--];
}

//
// stackint_push - 压入元素到对象栈栈顶
// s        : stackint 对象栈
// m        : 待压入的对象
// return   : void
// 
inline void stackint_push(struct stackint * s, int m) {
    if (s->cap <= s->tail) {
        s->cap <<= 1;
        s->data = realloc(s->data, sizeof(int)*s->cap);
    }
    s->data[++s->tail] = m;
}

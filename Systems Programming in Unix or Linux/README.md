# Unix / Linux 系统编程 (K.C.Wang 著 肖堃译) 读书笔记

这本书读起来**非常舒服**和**平易近人**. 想为这本黑皮书补充读书笔记, 也欢迎更多能手补充和完善. 因为自己英文零基础, 目前只以中文版样板. 对于英文版没有考证. 

读书笔记会按照**章节顺序**纪录. 并且设置 **[info]** 交流; **[warning]** 警告; **[error]** 错误三类标识. 

具体开篇之前, 有必要介绍下工程代码和科学学习代码不太一样地方. 工程代码非常考量**命名**, 因为他涉及到多人各种场景下合作为了传承和方便理解, **命名**是不能太随意, 需要一点辨识度和花心思考量在里面; 其次本文主要 C 去描述的, 而在工程 C 开发中最重要环节之一就是**内存管理**, 简单的说 malloc / free, new / delete, create / release 最好对称, 防止内存泄漏的代码是生产环境代码中重中之重.  

## 1

### [info] 1.12 虚拟机上的 Linux

学习 Linux 最好直接安装 Linux 物理机去学习最好, 其次推荐用虚拟机去学习. 这里补充推荐一个 **window 11 的 wsl2** 自带虚拟化, 非常容易安装, 适合学生首次接触并学习 linux.  

[https://docs.microsoft.com/en-us/windows/wsl/install](https://docs.microsoft.com/en-us/windows/wsl/install)

## 2

### [info] 2.1 Linux 中的文本编辑器

这里补充推荐 **Viusal Studio Code** 可以尝试尝试很舒服, 简洁高效强大. 

### [warning] 2.4.2 long jump

用 ```gcc -g -Wall -o longjump longjump.c``` 编译 warning 很多呢😢! 

- warning: implicit declaration of function ‘A’ [-Wimplicit-function-declaration]
- warning: control reaches end of non-void function [-Wreturn-type]
- warning: conflicting types for ‘A’

原文代码是这样的

```C
/** longjump.c file: demonstrate long jump in Linux  */
#include <stdio.h>
#include <setjmp.h>
jmp_buf env; // for saving longjmp environment

int main(void) {
    int r, a = 100;
    printf("call setjmp to save environment\n");
    if ((r = setjmp(env)) == 0) {
        A();
        printf("normal return\n");
    } else
        printf("back to main() via long jump, r=%d a=%d\n", r, a);
}

int A() {
    printf("enter A()\n");
    B();
    printf("exit A()\n");
}

int B() {
    printf("enter B()\n");
    printf("long jump? (y | n) ");
    if (getchar() == 'y')
        longjmp(env, 1234);
    printf("exit B()\n");
}

```

给出一个修改 fix 版本

```C
/** longjump.c file: demonstrate long jump in Linux  */
#include <stdio.h>
#include <setjmp.h>

jmp_buf env; // for saving longjmp environment

void B() {
    printf("enter B()\n");
    printf("long jump? (y | n) ");
    if (getchar() == 'y')
        longjmp(env, 1234);
    printf("exit B()\n");
}

void A() {
    printf("enter A()\n");
    B();
    printf("exit A()\n");
}

int main(void) {
    int r, a = 100;
    printf("call setjmp to save environment\n");
    if ((r = setjmp(env)) == 0) {
        A();
        printf("normal return\n");
    } else {
        printf("back to main() via long jump, r=%d a=%d\n", r, a);
    }
    return 0;
}

```

工程代码很要求 **严谨可靠** 最好 **零 warning**. K&R C 很古老, 推荐学习和融入 C2X 标准中, 享受现代化 C 的编程乐趣呢. 在规范之后之后也可以再在变量命名上下功夫. 

后面代码大量 ```int func() { /* no return */ }``` 情况, 不在一一列举. 推荐文中代码里, **每个 int 开头 func 都应该 review 一遍** ~ 考量下这么写到底要引导干什么呢?

### [warning] 2.8.3

```C
int x, *p;          // or int *p = &x;
int main() {
    int *q;
    p = &x;         // let p point at x
    *p = 1;
    q = (int *)malloc(sizeof(int)); // q point at allocate memory
    *q = 2;
}
```

修正下这段介绍代码

```C
#include <stdlib.h>

int x, * p;         // or int *p = &x;

int main(void) {
    int * q;
    p = &x;         // let p point at x
    *p = 1;
    q = malloc(sizeof(int)); // q point at allocate memory
    *q = 2;
    
    free(q);
    exit(EXIT_SUCCESS);
}

```

这里执行完 malloc 之后进程就退出了, 所以没有泄露. 但对于很多不懂初学者, 需要训练他们避坑. 推荐代码, **malloc / free** 成对出现. 包括整本书所有出现 **malloc 地方推荐仔细 review code**. 

### [info] 2.10.3 构建链表

截取部分书中代码

```C
node = (NODE *)malloc(N*sizeof(NODE)); // node->N*72 bytes in HEAP
for (i = 0; i < N; i++) {
    p = &node[i];       // access each NODE area in HEAP
    sprintf(p->name, "%s%d", "node", i);
    p->id = i;
    p->next = p+1;      // node[i].next = &node[i+1]
}
node[N-1].next = 0;
```

这里 ```p->next=p+1``` 最后在 i = N-1 时候, p+1 是野指针, 不应该被赋值给其他变量. 工程代码大部分四平八稳, 稳定性和规矩放在首位.

```C
int i;
NODE * p;

assert(N >= 1);
node = malloc(N * sizeof(NODE));

p = &node[N-1];
sprintf(p->name, "%s%d", "node", N-1);
p->id = N-1;
p->next = NULL;

for (i = 0; i < N-1; i++) {
    p = &node[i];       // access each NODE area in HEAP
    sprintf(p->name, "%s%d", "node", i);
    p->id = i;
    p->next = p+1;      // node[i].next = &node[i+1]
}

... .. .

free(node);
```

更加严谨点需要对 sprintf 返回值做判断, 看是否有错误. 这里需要交流, 正确没有歧义是基准. 另外也有个问题需要交流 **这章 NODE 命名定义太多了, 非常绕** 也许 **list_node, queue_node, double_list_node** 命名会好一点. 

### [info] 2.10.8 删除操作

```NODE * delete(NODE ** list, int key)``` 这样定义来回看大致也能看出来 NODE 内部有什么字段, 干什么用的. 因为每处 NODE 不一样. 关于这个 key 前面也没有介绍. 严谨点, 应该约定 这里用的 key 是 unquie **唯一不重复**的. 否则代码没法保证正确性.

### [error] 2.12.5 广度优先遍历算法

首先定义这个树结构(也可能是代码不全), 有 new 代码, 没有 delete 删除整体树的代码. 这个问题前面也说过, 不继续重复. 来看 printLevel 实现, 因为内部执行了 enqueue 操作, 每次都 malloc, 缺少 free 操作. 会造成内存泄露. 修订如下:

```C
// print a binary tree by levels, each level on a line
void printLevel(NODE * root) {
    int nodeCount;

    if (root == NULL) return;

    QE * queue= NULL; // 书中是 QE queu = 0; 特定错了, 可能是中文版问题
    enqueue(&queue, root);
    for (;;) {
        nodeCount = qlength(queue);
        if (nodeCount == 0) break;
        while (nodeCount > 0) {
            NODE * node = dequeue(&queue);
            printf("%d ", node->key);
            if (node->left != NULL)
                enqueue(&queue, node->left);
            if (node->right != NULL)
                enqueue(&queue, node->right);
            nodeCount--;

            // 添加防止泄露语句
            free(node);
        }
    }
}
```

这么写代码非常累. 推荐用下面方式

```C
NODE * dequeue(QE ** queue) {
    QE * q = *queue;
    if (q == NULL) {
        return NULL;
    }
    *queue = q->next;
    return q->node;
}

// print a binary tree by levels, each level on a line
void printLevel(NODE * root) {
    if (root == NULL) return;

    QE * queue= NULL;
    enqueue(&queue, root);

    NODE * node;

    while ((node = dequeue(&queue)) != NULL) {
        printf("%d ", node->key);

        if (node->left != NULL)
            enqueue(&queue, node->left);
        if (node->right != NULL)
            enqueue(&queue, node->right);

        free(node);
    }
}
```

## 3

### [warning] 3.10.1 Unix/Linux 中管道编程

C3.7 给了一个例子, 可以参照, 重点是 **close 主动释放文件描述符**.

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -g -Wall -o pipe2 pipe2.c
int main(void) {
    int pd[2];
    // create a pipe
    if (pipe(pd)) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    printf("pd = [%d, %d]\n", pd[0], pd[1]);

    int child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    // fork a child to share the pipe
    if (child) {
        printf("parent %d close pd[0]\n", getpid());
        close(pd[0]); 
        for (int i = 0; i < 10; i++) {
            printf("parent %d writing to pipe\n", getpid());
            int n = write(pd[1], "I AM YOUR PAPA", 16);
            printf("parent %d wrote %d bytes to pipe\n", getpid(), n);
        }
        printf("parent %d exit\n", getpid());
        close(pd[1]);
    } else {
        printf("child %d close pd[1]\n", getpid());
        close(pd[1]);
        for (int i = 0; i < 10; i++) {
            char line[256];
            printf("child %d reading from pipe\n", getpid());
            int n = read(pd[0], line, 255);
            if (n <= 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            line[n] = 0;
            printf("child read %d bytes from pipe: %s\n", n, line);
        }
        close(pd[0]);
    }

    exit(EXIT_SUCCESS);
}

```

## 6

### [warning] 6.6 信号用作 IPC

这里 jmp_buf 和 sigaction 演示代码毛病太多, 推荐用下面这版本代码

```C
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>
#include <stdatomic.h>

#include <unistd.h>

#ifndef UNUSED
#define UNUSED(parameter) if (parameter) {}
#endif//UNUSED

static jmp_buf env;
static atomic_int count;

static void BAD(void) {
    int * ip = NULL;
    
    printf("in BAD(): try to dereference NULL pointer\n");

    *ip = 123;

    printf("should not see this line\n");
}

static void handler_sigsegv(int sig, siginfo_t * info, void * ucontext) {
    UNUSED(ucontext);

    int n = atomic_fetch_add(&count, 1)+1;

    // [error] 不应该在信号中使用 printf 等不可重入函数
    printf("handler: sig = %d from PID = %d, UID = %d count = %d\n", sig, info->si_pid, info->si_uid, n);

    if (n >= 4) {
        longjmp(env, 1234);
    }
}

// gcc -O2 -g -Wall -Wextra -Werror -o trycatch trycatch.c
int main(void) {
    int res;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = handler_sigsegv;
    act.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGSEGV, &act, NULL)) {
        perror("sigaction SIGSEGV : ");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        if ((res = setjmp(env)) == 0) {
            BAD();
        } else {
            printf("proc %d survived SEGMENTATION FAULT: res = %d\n", getpid(), res);
            break;
        }
    }

    printf("proc %d looping\n", getpid());
    exit(EXIT_SUCCESS);
}

```

## 8

### [error] 8.6.5 opendir-readdir 函数

opendir 和 closedir 需要成双成对防止内存泄漏.

## 9

### [info] 9.2 I/) 库函数与系统调用

BLKSIZE 其实没有必要, 可以直接系统定义的 BUFSIZ, 少定义哥宏呢.

## 13

### 13.15 TCP 回显服务器 - 客户机程序

`listen(mysock, 5);` 也应该判断返回值是否是 -1 . 其次 `int main()` 中需要主动关闭 mysock. 更多的应该推荐学生阅读更专业书籍, 对于网络编程进行扩展阅读, 因为他是现代编码中一个大分支, 社会中会写的少.

## 14

### [info] 14.4.4 使用 C 语言检索 MySQL 查询结果

代码中 `mysql_query -> mysql_store_result` 之后需要 `mysql_free_result` 推荐加上, 培养学生严谨工程思维.
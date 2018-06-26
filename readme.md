---
typora-copy-images-to: ./images
typora-root-url: .
---


# 文件读写编程题目

## myecho.c

> - myecho.c的功能与系统echo程序相同
> - 接受命令行参数，并将参数打印出来

source code

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    // printf("arguments number is : %d\n", argc - 1);
    int i;
    for (i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

```
argc代表命令行中参数个数, 包括文件名本身(第0个参数). argv则包含命令行的输入. 可以使用一个loop按顺序输出

![1530001770616](/images/1530001770616.png)

## mycat.c

> - mycat.c的功能与系统cat程序相同
> - mycat将指定的文件内容输出到屏幕，例子如下：
> - 要求使用系统调用open/read/write/close实现

core code

```c
    file = open(path, O_RDONLY);
    if (file == -1)
    {
        printf("unable to open %s", argv[1]);
    }
    char *buf[100000];
    int length;
    length = read(file, buf, 100000);
    if (length != -1)
    {
        printf("%s", buf);
        // printf("%d", length);
    }
```

将argv第1个参数, 也就是文件名读入, 然后打开, 读入到字符数组并输出

![1530001720138](/images/1530001720138.png)

## mycp.c

> - mycp.c的功能与系统cp程序相同
> - 将源文件复制到目标文件，例子如下：
> - 要求使用系统调用open/read/write/close实现

core code1

```c
     char *path = argv[1];
     int file;
      file = open(path, O_RDONLY);
      if (file == -1) {
          printf("unable to open %s", argv[1]);
      }
      char *buf[100000];
      int length;
```

现从argv[1]对应的文件路径中读入文件, 这里的length会得到read的返回值: **实际读取的字节个数**, 便于后面重新写到新的文件中.

```c
      length = read(file, buf, 100000);
      if (length != -1){
          // printf("%s", buf);
          int file_to_write;
          file_to_write =  open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
          if (write(file_to_write, buf, length) != -1) {
              printf("finish write file!!!");
          }
          else {
              printf("unable to write %s", argv[2]);
          }
          close(file_to_write);
```

file_to_write即是待写入的文件, 写入length字节长的buf, 注意这里的length很关键, 不然会有乱码,  因为读入的buf超过了原文件本身.

![1529941197893](./images/1529941197893.png)

# 多进程题目

## mysys.c 

> - 实现函数mysys，用于执行一个系统命令，要求如下
> - mysys的功能与系统函数system相同，要求用进程管理相关系统调用自己实现一遍
> - 使用fork/exec/wait系统调用实现mysys
> - 不能通过调用系统函数system实现mysys

**core** code

```c
    if ((pid = fork()) < 0)
    {
        status = -1;
    }
    else if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", cmdstring, NULL);
        _exit(127);
    }
    else
    {
        while (waitpid(pid, &status, 0) < 0)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }
    }
```

这里调用fork()后, 创建一个子进程，父子进程并发运行, 子进程返回0, 进入第一个分支, 调用execl, 参数是主函数传来需要执行的命令, 父进程返回的子进程PID, 不是零, 进入下一条分支, 等待子进程执行完, 结束程序.

![1530001551413](/images/1530001551413.png)

## sh1.c

> * 实现shell程序，要求具备如下功能
> * 支持命令参数
> * 实现内置命令cd、pwd、exit

core code 1

```c
void split(char buf[], char *array[]) {
    int i = 0;
    char *p = strtok(buf, " ");

    while (p != NULL) {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
    for (; i < N_ARRAY; i++) {
        array[i] = NULL;
    }
}

```

用于切分字符串buf, 以空格为界, 存入新的字符指针数组array.

core code 2

```c
    while (1) {
        char command[LEN];
        char *array[N_ARRAY];

        printf("%s", ">>");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0'; // ignore newline!!!
        strcpy(command_fix, command);
        split(command, array);
        /*for (int j = 0; j < 10; j++) {
            printf("***%s***%d \n", array[j], array[j] == NULL);
        }*/
        if (-1 == execute(array))
            return 0;
    }
```

为了构造类似shell的环境, 需要进入一个无限循环, 直到输入exit, execute函数返回-1时, 才会退出. 循环内split输入命令, 调用execute函数来执行.

core code 3

```c
    if (strcmp(array[0], "echo") == 0) {
        int pid = fork();
        if (pid == 0) {
            execl("/bin/sh", "sh", "-c", command_fix, NULL);
        } else {
            int status;
            wait(&status);
        }
    } else if (strcmp(array[0], "ls") == 0) {
        int pid = fork();
        if (pid == 0) {
            if (array[1] == NULL) {
                execlp("/bin/ls", "ls", "./", NULL, NULL, NULL);
            } else {
                execlp("/bin/ls", "ls", array[1], NULL, NULL, NULL);
            }
        } else {
            int status;
            wait(&status);
        }
    } else if (strcmp(array[0], "exit") == 0) {
        return -1;
    } else if (strcmp(array[0], "cd") == 0) {
        chdir(array[1]);
    } else if (strcmp(array[0], "pwd") == 0) {
        int pid = fork();
        if (pid == 0) {
            printf("test pwd!!!\n");
            system("pwd");
        } else {
            int status;
            wait(&status);
        }
    }
```

以上使用实现了echo, ls, exit, cd, pwd的功能, 每个功能实现流程都是如下:

* 对分割后的array数组中的第一个字符串判断, 是否是要实现的功能
* 若是则使用fork(), 创建子线程实现
* 主线程等待子线程结束

运行截图

![1529944245906](./images/1529944245906.png)

## sh2.c 

> * 实现shell程序，要求在第1版的基础上，添加如下功能
>
> - 实现文件重定向

使用excel执行echo命令, 传入参数已经经过split函数切分, 可以直接实现重定向功能

```c
    if (strcmp(array[0], "echo") == 0) {
        int pid = fork();
        if (pid == 0) {
            execl("/bin/sh", "sh", "-c", command_fix, NULL);
        } else {
            int status;
            wait(&status);
        }
```

![1530002097205](/images/1530002097205.png)

## sh3.c

> - 实现shell程序，要求在第2版的基础上，添加如下功能
> - 实现管道
> - 实现管道和文件重定向

需要对split函数改进做到能处理管道符, 需要重写split函数, 再split函数内调用新的函数split_pipe来处理管道符

```c
	char sub[] = "|";
	cmd_total = 0;
	p = strtok(command,sub);
	if(p!=NULL){
		cmd[0][0]=p;
		++cmd_total;
	}
	while((p=strtok(NULL,sub))){
		cmd[cmd_total][0]=p;
		++cmd_total;
	}
```

处理完毕后可以继续使用execl运行命令.

# 多线程题目

## pi1.c 

> - 使用2个线程根据莱布尼兹级数计算PI
> - 莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
> - 主线程创建1个辅助线程
> - 主线程计算级数的前半部分
> - 辅助线程计算级数的后半部分
> - 主线程等待辅助线程运行結束后,将前半部分和后半部分相加

core code 1

```c
float res = 0;
struct arg_struct {
    int arg1;
    int arg2;
};
```

这里的float型res是最终结果存放的位置, 全局变量. arg_struct则是定义的结构体, 作为存放线程的**参数**.

core code 2

```c
void *compute(void *arg) {
    struct arg_struct *me;
    me = (struct arg_struct *) arg;
    int start = me->arg1;
    int end = me->arg2;
    for (; start <= end; start++)
        res += powf(-1, start + 1) / (2 * start - 1);
}
```

首先将参数转换为定义的参数类型, 解析出起始和结束的序列号, 按公式计算并加到res中.

core code3

```c
    pthread_t worker_tid;
    struct arg_struct *parent = malloc(sizeof(struct arg_struct));
    struct arg_struct *child = malloc(sizeof(struct arg_struct));
    parent->arg1 = 1;
    parent->arg2 = n / 2;
    child->arg1 = n / 2 + 1;
    child->arg2 = n;
    pthread_create(&worker_tid, NULL, &compute, child);
    compute(parent);
    pthread_join(worker_tid, NULL);
```

以上定义了父子线程的参数, 并创建子线程, 父线程同时执行compute, 等待子线程结束.

运行截图

![1529945524620](./images/1529945524620.png)

## pi2.c 

> - 使用N个线程根据莱布尼兹级数计算PI
> - 与上一题类似，但本题更加通用化，能适应N个核心，需要使用线程参数来实现
> - 主线程创建N个辅助线程
> - 每个辅助线程计算一部分任务，并将结果返回
> - 主线程等待N个辅助线程运行结束，将所有辅助线程的结果累加

本题相较于上一题, 线程数量变多, 需要使用数组来存放线程id与线程参数, 需要使用循环来对参数初始化并创建线程, 并且线程需要返回值

core code 1

```c
struct arg_struct {
    int start;
    int end;
};
struct result {
    float sum;
};
```

以上是定义的参数结构题, 返回值的结构体

core code 2

```c
    pthread_t workers[NR_CPU];
    struct arg_struct args[NR_CPU];
    int p = 1;
    for (i = 0; i < NR_CPU; i++) {
        struct arg_struct *arg;
        arg = &args[i];
        arg->start = p;
        if (p + n / NR_CPU - 1 <= n) {
            arg->end = p + n / NR_CPU - 1;
        } else {
            arg->end = n;
            break;
        }
        pthread_create(&workers[i], NULL, &compute, arg);
        p += (n / NR_CPU);
    }
```

这里workers数组存放多个线程id, arg_struct数组存放每个线程的参数, 使用下方的循环做初始化, 即找到每个线程中计算$\pi$值的开始和结束序列号, 并且创建线程.

core code 3

```c
void *compute(void *arg) {
    struct arg_struct *me;
    me = (struct arg_struct *) arg;
    int start = me->start;
    int end = me->end;
    float sum = 0;
    for (; start <= end; start++)
        sum += powf(-1, start + 1) / (2 * start - 1);
    struct result *res;
    res = malloc(sizeof(struct result));
    res->sum = sum;
//    printf("**  %f\n", res->sum);
    return res;
}
```

定义计算$\pi$值的函数, 在上题基础上增加返回值功能.

core code 4

```c
    float sum = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **) &result);
        sum += result->sum;
//        printf("result->sum = %d\n", result->sum);
        free(result);
    }
```

线程同步, 用一个循环确保每个子线程都结束运行, 并且返回值存在result中, 累加得到的sum即为$\pi/4$.

运行截图:

![1529946873730](/images/1529946873730.png)

## pc1.c 

> - 使用条件变量解决生产者、计算者、消费者问题
> - 系统中有3个线程：生产者、计算者、消费者
> - 系统中有2个容量为4的缓冲区：buffer1、buffer2
> - 生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
> - 计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
> - 消费者从buffer2取出字符，将其打印到屏幕上

这个题重点在于弄清生产者, 计算者, 消费者的运行逻辑以及互斥关系, 计算者要从生产者存放item的buffer1中get item, 并进行计算, 大小写转换后放入buffer2, 消费者从buffer2中再读取利用.

![1529993827193](/images/1529993827193.png)

需要定义buffer1_is_empty, buffer2_is_empty函数来判断buffer1和buffer2是否为空, buffer1_is_full, buffer2_is_full函数来判断buffer1和buffer2是否满. 定义函数put_item_to_buffer1用来将item放入buffer1, 定义put_item_to_buffer2用来从buffer1取item并转换为大写, 再放入buffer2. 函数get_item_from_buffer2用来从buffer2取item.

core code 1

```c
void *compute(void *arg)
{
	int i;

	for (i = 0; i < ITEM_COUNT; i++)
	{
		pthread_mutex_lock(&mutex1);
		pthread_mutex_lock(&mutex2);
		while (buffer1_is_empty())
			pthread_cond_wait(&wait_full_buffer1, &mutex1);
		while (buffer2_is_full())
			pthread_cond_wait(&wait_empty_buffer2, &mutex2);

		put_item_to_buffer2();
		// printf("  compute item");

		pthread_cond_signal(&wait_empty_buffer1);
		pthread_cond_signal(&wait_full_buffer2);
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}
```

上面的代码是消费者的函数, 用两个信号量mutex1和mutex2分别与生产者, 消费者互斥, wait_full_buffer1则表示当buffer1空时需要在此条件变量上阻塞. 同样的, 当buffer2满时需要在条件变量wait_empty_buffer2上阻塞.

core code 2

```c
void *consume(void *arg)
{
	int i;
	int item;

	for (i = 0; i < ITEM_COUNT; i++)
	{
		pthread_mutex_lock(&mutex2);
		while (buffer2_is_empty())
			pthread_cond_wait(&wait_full_buffer2, &mutex2);
		item = get_item_from_buffer2();
		printf("    consume item: %c\n", item);

		pthread_cond_signal(&wait_empty_buffer2);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}
```

以上为消费者函数, 用mutex2来保证consume运行时compute不会执行. 当buffer2为空时, 无法继续消费buffer2, 线程在条件变量wait_full_buffer2上阻塞.

运行截图

![1529994468467](/images/1529994468467.png)

## pc2.c

> - 使用信号量解决生产者、计算者、消费者问题
> - 功能和前面的实验相同，使用信号量解决

线程的制约关系与上题完全相同, 不过不再使用条件变量而是用信号量机制实现.

![1529993827193](/images/1529993827193.png)

可以用条件变量实现信号量机制

core code 1

```c
void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}
```

这里的value小于等于零是线程将会阻塞, 调用signal则value++.

core code 2

```c
void *compute(void *arg)
{
    int i;
    for (i = 0; i < ITEM_COUNT; i++)
    {

        sema_wait(&full_buffer1_sema);
        sema_wait(&empty_buffer2_sema);

        sema_wait(&mutex1_sema);
        sema_wait(&mutex2_sema);

        put_item_to_buffer2();

        sema_signal(&mutex1_sema);
        sema_signal(&mutex2_sema);

        sema_signal(&empty_buffer1_sema);
        sema_signal(&full_buffer2_sema);
    }
    return NULL;
}
```

计算的线程里, 需要先对full_buffer1_sema信号量做P操作, 防止buffer1为空时仍然去取数据, 因此在put_item_to_buffer2执行结束后需要对empty_buffer1_sema信号量做V操作, 因为计算时取了一个item, 故要从增加buffer1中一个空缓冲区.

同理, 对empty_buffer2_sema做P操作是为了确保buffer2中有空缓冲区, 执行完put_item_to_buffer2后对full_buffer2_sema做V操作, 因为buffer2中会多一个满缓冲区.

信号量mutex1_sema与mutex2_sema是为了互斥的对buffer1和buffer2进行操作.

core code 2

```c
    pthread_t computer_tid;
    // pthread_t producer_tid;
    pthread_t consumer_tid;

    sema_init(&mutex1_sema, 1);
    sema_init(&mutex2_sema, 1);
    sema_init(&empty_buffer1_sema, CAPACITY - 1);
    sema_init(&empty_buffer2_sema, CAPACITY - 1);
    sema_init(&full_buffer1_sema, 0);
    sema_init(&full_buffer2_sema, 0);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();

    pthread_join(consumer_tid, NULL);
    pthread_join(computer_tid, NULL);
    return 0;
```

以上是对信号量初始化, 以及创建两个子线程分别运行运行消费者线程和计算者线程, 主线程运行生产者线程, 然后线程同步的流程.

运行截图

![1529999892678](/images/1529999892678.png)

## ring.c

> - 创建N个线程，它们构成一个环
> - 创建N个线程：T1、T2、T3、… TN
> - T1向T2发送整数1
> - T2收到后将整数加1
> - T2向T3发送整数2
> - T3收到后将整数加1
> - T3向T4发送整数3
> - …
> - TN收到后将整数加1
> - TN向T1发送整数N

关于这道题目题意的理解:

1. 对于*"T2向T3发送整数2"*这句话, 如果理解为: 这个发送的过程在线程T2内进行, 则需要再线程T2内创建新线程T3并执行
2. 对于*"T2向T3发送整数2"*这句话, 如果理解为: T2收到来自T1的整数后, 在T2内自加, 然后在主线程内创建新线程T3并传入参数, 则需要再main函数内使用一个数组来存放所有线程, 用循环的方式实现, 类似于pi2.c的方法.

以下的做法基于第一种理解.

core code 1

```c
struct param
{
    int a;
};
```

先定义参数类型, 并且返回值也可以是这个结构体类型.

core code 2

```c
    pthread_t start_id;
    struct param *para;
    para = (struct param *)malloc(sizeof(struct param));
    para->a = 1;
    struct para *res;
    // res = (struct param *)compute(para);
    pthread_create(&start_id, NULL, compute, para);
    pthread_join(start_id, NULL);
    return 0;
```

在主函数中创建初始线程以及所需的结构体参数, 并且变量赋值为1.

core code 3

```c
int count = 1;
void *compute(void *arg)
{
    count += 1;
    struct param *para;
    para = (struct param *)arg;
    para->a += 1;
    if (count < N)
    {
        pthread_t next_id;
        pthread_create(&next_id, NULL, compute, para);
        pthread_join(next_id, NULL);
        return NULL;
    }
    else
    {
        printf("Finally, the first thread receives %d", para->a);
    }
}
```

线程函数compute内对参数加1, 创建新的线程, 传入自加后的参数.

运行截图

![1530001359294](/images/1530001359294.png)
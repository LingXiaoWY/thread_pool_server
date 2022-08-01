/*###################################################################
# File Name: Thread_pool_server.h
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 10:18:26 AM CST
#=============================================================*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<errno.h>
#include<sys/epoll.h>
#include<time.h>
#include<ctype.h>

#define _BACKLOG 128
#define _EPOLLMAX 100000
#define _BUFSIZE 1500
#define _TRUE 1
#define _FALSE 0
#define _DEFCODE 10
#define _IPSIZE 16
#define _IP "192.168.3.133"
#define _PORT 8080
#define _TIMEOUT 1

pthread_mutex_t accept_lock;
int epfd;
//任务类型
typedef struct{
	void* (*business)(void*);
	void* arg;
}task_t;

//线程池类型
typedef struct{
	int thread_max;//线程最大数
	int thread_min;//线程最小数
	int thread_busy;//繁忙线程数
	int thread_alive;//有效线程数
	int thread_shutdown;//线程池开关
	int thread_exitcode;//缩减码
	int queue_max;//任务队列最大数
	int queue_cur;//任务队列当前任务数
	int queue_front;//任务队列头索引
	int queue_rear;//任务队列尾索引
	task_t* queue_list;//任务队列地址
	pthread_mutex_t lock;//互斥锁
	pthread_cond_t not_full;//生产者条件变量
	pthread_cond_t not_empty;//消费者条件变量
	pthread_t *Ctids;//存储消费者线程id
	pthread_t Mtid;//存储管理者线程id
}pool_t;

void thread_str_error(const char* errstr,int exitcode,int flag);//错误处理函数，既可以处理系统函数的错误(perror),也兼容线程错误处理(strerror)
int thread_pool_destroy(pool_t* ptr);//线程池销毁
pool_t* thread_pool_create(int tmax,int tmin,int qmax);//线程池创建初始化
int producer_add_task(pool_t* ptr,task_t task);//生产者任务（添加）
void* thread_customer(void*);//消费者任务
void* thread_manager(void*);//管理者任务
int test_thread_alive(pthread_t tid);//测试线程是否存活
void* Business_Accept(void*);//连接业务
void* Business_Response(void*);//处理业务
int thread_pool_netinit(void);//网络创建初始化
int thread_epoll_init(int sockfd);//epoll创建初始化
int thread_epoll_listen(pool_t* ptr,int sockfd);//Epoll监听
int Server_StartUp(void);//服务器启动

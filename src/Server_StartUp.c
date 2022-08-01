/*
###################################################################
# File Name: Server_StartUp.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sun 31 Jul 2022 09:53:40 AM CST
===================================================================
*/
#include<Thread_pool_server.h>

int Server_StartUp(void)
{
	pool_t *ptr = NULL;
	int serverfd;
	printf("Thread pool Server [EPOLL] Version 1.10 Running..\n");
	pthread_mutex_init(&accept_lock,NULL);//初始化连接锁
	ptr = thread_pool_create(300,10,1000);//线程池创建初始化
	serverfd = thread_pool_netinit();//网络初始化
	thread_epoll_init(serverfd);//Epoll创建初始化，设置第一个sock监听
	thread_epoll_listen(ptr,serverfd);//生产者EPoll监听事件并添加任务
	return 0;
}

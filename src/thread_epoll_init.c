/*###################################################################
# File Name: thread_epoll_init.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 10:41:28 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

int thread_epoll_init(int sockfd)
{
	if((epfd=epoll_create(_EPOLLMAX))==-1)
		thread_str_error("thread_epoll_init ERROR epoll_create call Failed",1,0);
	struct epoll_event node;
	node.data.fd = sockfd;
	node.events = EPOLLIN|EPOLLET;//EPOLLIN 监听读事件，EPOLLET 边缘触发
	if((epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&node))==-1)
		thread_str_error("thread_epoll_init ERROR,epoll_ctl first add Serverfd Failed",1,0);
	printf("[Initializer...] Epoll Create Success Epfd[%d]\n",epfd);
	return 0;
}

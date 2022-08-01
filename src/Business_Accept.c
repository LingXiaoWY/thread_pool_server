/*###################################################################
# File Name: Business_Accept.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 11:29:22 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

void* Business_Accept(void* arg)
{
	int Serverfd = *(int *)arg;
	int clientfd;
	struct epoll_event node;
	struct sockaddr_in clientAddr;
	socklen_t Addrlen;
	char ip[_IPSIZE];
	bzero(ip,_IPSIZE);
	Addrlen = sizeof(clientAddr);
	pthread_mutex_lock(&accept_lock);
	if((clientfd = accept(Serverfd,(struct sockaddr*)&clientAddr,&Addrlen))==-1)
	{
		thread_str_error("Business_Accept ERROR, Accept call Failed",1,0);
	}
	pthread_mutex_unlock(&accept_lock);
	node.data.fd = clientfd;
	node.events = EPOLLIN|EPOLLET;
	if((epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&node))==-1)
		thread_str_error("Business_Accept ERROR,epoll_ctl Add Listen Clientfd Failed",1,0);
	printf("[Customer 0x%x] Exec Business_Accept Success,New Clientfd [%d] ClientIP[%s] ClientPort [%d]\n",(unsigned int)pthread_self(),clientfd,inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,ip,16),ntohs(clientAddr.sin_port));
	return NULL;
}

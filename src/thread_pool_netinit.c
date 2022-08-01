/*###################################################################
# File Name: thread_pool_netinit.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 09:59:16 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

int thread_pool_netinit(void)
{
	int sockfd;
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,_IP,&serverAddr.sin_addr.s_addr);
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
		thread_str_error("thread_pool_netinit ERROR,socket create Failed",1,0);
	if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==-1)
		thread_str_error("thread_pool_neinit ERROR,Bind Failed",1,0);
	if((listen(sockfd,_BACKLOG))==-1)
		thread_str_error("thread_pool_netinit ERROR Listen Failed",1,0);
	printf("[Initializer..] Thread pool Net init Success %d\n",sockfd);
	return sockfd;
}

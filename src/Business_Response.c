/*###################################################################
# File Name: Business_Response.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sun 31 Jul 2022 09:11:51 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

void *Business_Response(void *arg)
{
	int clientfd = *(int*)arg;
	ssize_t recvlen;
	char buffer[_BUFSIZE];
	char time_buffer[1024];
	int flag;
	time_t tp;
	bzero(time_buffer,1024);
	bzero(buffer,_BUFSIZE);
	while((recvlen = recv(clientfd,buffer,sizeof(buffer),MSG_DONTWAIT))>0){//非阻塞读取IO
		if((strcmp(buffer,"timeofday\n"))==0){
			tp = time(NULL);
			ctime_r(&tp,time_buffer);
			send(clientfd,time_buffer,strlen(time_buffer),MSG_NOSIGNAL);
			bzero(time_buffer,1024);
		}
		else{
			flag = 0;
			while(recvlen > flag){
				buffer[flag] = toupper(buffer[flag]);
				flag++;
			}
			send(clientfd,buffer,strlen(buffer),MSG_NOSIGNAL);//忽略信号
		}
		printf("[Customer 0x%x] Exec Business_Response, return Response to Client!\n",(unsigned int)pthread_self());

	}
	if(recvlen == -1){
		if(errno == EAGAIN)return NULL;
		thread_str_error("Business_Response ERROR,recv Request Failed",1,0);
	}
	if(recvlen == 0){
		printf("[Customer 0x%x] Client exiting,close Clientfd [%d]\n",(unsigned int)pthread_self(),clientfd);
		epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,NULL);//删除监听节点
		close(clientfd);
	}
	return NULL;
}

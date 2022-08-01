/*###################################################################
# File Name: thread_epoll_listen.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 11:00:52 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

int thread_epoll_listen(pool_t* ptr,int sockfd)
{
	int readycode;
	struct epoll_event readyArray[_EPOLLMAX];
	task_t task;
	int flag;
	printf("[listen...] Epoll listen Socket Events\n");
	while(ptr->thread_shutdown){
		flag = 0;
		if((readycode=epoll_wait(epfd,readyArray,_EPOLLMAX,-1))==-1)
			thread_str_error("thread_epoll_listen ERROR,epoll_wait call Failed",1,0);
		while(readycode){
			//辨别就绪，添加对应的绑定业务
			if(readyArray[flag].data.fd == sockfd){
				task.business = Business_Accept;
				task.arg = (void*)&sockfd;
				producer_add_task(ptr,task);//添加连接业务
				printf("[Producer 0x%x] add business_accept to Container..\n",(unsigned int)(pthread_self()));
			}else{
				task.business = Business_Response;
				task.arg = (void*)&(readyArray[flag].data.fd);
				producer_add_task(ptr,task);//添加处理业务
				printf("[Producer 0x%x] Add Business_Response to Container..\n",(unsigned int)(pthread_self()));
			}
			++flag;
			--readycode;
		}
	}
	close(sockfd);
	return 0;
}

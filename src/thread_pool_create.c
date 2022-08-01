/*###################################################################
# File Name: thread_pool_create.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Fri 29 Jul 2022 10:10:06 PM CST
#=============================================================*/
#include<Thread_pool_server.h>

//线程池创建初始化
pool_t* thread_pool_create(int tmax,int tmin,int qmax)
{
	pool_t* ptr = NULL;
	if((ptr=(pool_t*)malloc(sizeof(pool_t)))==NULL)
		thread_str_error("thread_pool_create ERROR,Malloc Pool Failed",1,0);
	
	ptr->thread_max = tmax;
	ptr->thread_min = tmin;
	ptr->thread_busy = 0;
	ptr->thread_alive = 0;
	ptr->thread_shutdown = _TRUE;
	ptr->thread_exitcode = 0;
	ptr->queue_max = qmax;
	ptr->queue_cur = 0;
	ptr->queue_front = 0;
	ptr->queue_rear = 0;
	if((ptr->queue_list = (task_t*)malloc(sizeof(task_t) * qmax))==NULL)
		perror("ptr->queue_list malloc failed");

	if((ptr->Ctids = (pthread_t*)malloc(tmax*sizeof(pthread_t)))==NULL)
		perror("ptr->Ctids malloc failed");

	bzero(ptr->Ctids,tmax*sizeof(pthread_t));//初始化Ctids
	//初始化锁和条件变量
	if(pthread_mutex_init(&ptr->lock,NULL)!=0||pthread_cond_init(&ptr->not_full,NULL)!=0||pthread_cond_init(&ptr->not_empty,NULL)!=0){
		printf("thread_pool_create ERROR,init lock or cond Failed\n");
		exit(1);
	}
	int err;
	for(int i = 0;i<tmin;i++){
		if((err=pthread_create(&ptr->Ctids[i],NULL,thread_customer,(void*)ptr))>0)
			thread_str_error("thread_pool_create ERROR,create customer thread Failed",2,err);
	}
	if((err=pthread_create(&ptr->Mtid,NULL,thread_manager,(void*)ptr))>0)
		thread_str_error("thread_pool_create ERROR, create Manager thread Failed",2,err);
	
	printf("[Initializer...] Thread Pool create\n");
	return ptr;
}

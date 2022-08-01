/*###################################################################
# File Name: thread_customer.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sat 30 Jul 2022 11:22:01 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

void *thread_customer(void* arg)
{
	pool_t *ptr = (pool_t*)arg;
	task_t tmp;
	printf("[Customer 0x%x] Wait Business...\n",(unsigned int)pthread_self());
	while(ptr->thread_shutdown)
	{
		pthread_mutex_lock(&ptr->lock);
		while(ptr->queue_cur == 0){
			pthread_cond_wait(&ptr->not_empty,&ptr->lock);
			if(ptr->thread_exitcode > 0){
				--(ptr->thread_alive);
				--(ptr->thread_exitcode);
				pthread_mutex_unlock(&ptr->lock);
				pthread_exit(NULL);
			}
		}
		//判断开关
		if(ptr->thread_shutdown==0){
			--(ptr->thread_alive);
			pthread_mutex_unlock(&ptr->lock);
			pthread_exit(NULL);
		}
		//获取任务并执行
		tmp.business = ptr->queue_list[ptr->queue_rear].business;
		tmp.arg = ptr->queue_list[ptr->queue_rear].arg;
		--(ptr->queue_cur);
		ptr->queue_rear = (ptr->queue_rear + 1) % ptr->queue_max;
		++(ptr->thread_busy);
		pthread_mutex_unlock(&ptr->lock);
		(tmp.business)(tmp.arg);//执行任务函数
		pthread_mutex_lock(&ptr->lock);
		--(ptr->thread_busy);
		pthread_mutex_unlock(&ptr->lock);
		pthread_cond_signal(&ptr->not_full);//唤醒生产者
	}
	pthread_exit(NULL);
}

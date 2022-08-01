/*###################################################################
# File Name: producer_add_task.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sat 30 Jul 2022 10:53:20 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

int producer_add_task(pool_t* ptr,task_t job)
{
	pthread_mutex_lock(&ptr->lock);
	if(ptr->thread_shutdown)
	{
		//生产者条件判断
		while(ptr->queue_cur == ptr->queue_max)
			pthread_cond_wait(&ptr->not_full,&ptr->lock);
		if(ptr->thread_shutdown == _FALSE){
			pthread_mutex_unlock(&ptr->lock);
			pthread_exit(NULL);
		}
		ptr->queue_list[ptr->queue_front].business = job.business;
		ptr->queue_list[ptr->queue_front].arg = job.arg;
		++ptr->queue_cur;
		ptr->queue_front = (ptr->queue_front+1)%ptr->queue_max;
		pthread_mutex_unlock(&ptr->lock);
		pthread_cond_signal(&ptr->not_empty);
		return 0;
	}
	pthread_mutex_unlock(&ptr->lock);
	return -1;
}

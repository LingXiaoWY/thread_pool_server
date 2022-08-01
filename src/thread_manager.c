/*###################################################################
# File Name: thread_manager.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sat 30 Jul 2022 12:08:31 PM CST
#=============================================================*/
#include<Thread_pool_server.h>

void *thread_manager(void *arg)
{
	pool_t *ptr = (pool_t *)arg;
	int cur;
	int alive;
	int busy;
	int err;
	int add;//记录线程创建数量
	int flag;//用于遍历Ctids
	printf("[Manager 0x%x] listen Thread pool state...\n",(unsigned int)pthread_self());
	while(ptr->thread_shutdown){
		//获取阈值
		pthread_mutex_lock(&ptr->lock);
		cur = ptr->queue_cur;
		alive = ptr->thread_alive;
		busy = ptr->thread_busy;
		pthread_mutex_unlock(&ptr->lock);
		//扩容条件
		if((cur >= alive - busy || (busy*100) / alive  >= 70)&& alive +_DEFCODE <= ptr->thread_max){
			for(add=0,flag=0;flag<ptr->thread_max && add<_DEFCODE;flag++){

				if(ptr->Ctids[flag]==0||!test_thread_alive(ptr->Ctids[flag])){
					if((err = pthread_create(&ptr->Ctids[flag],NULL,thread_customer,(void*)ptr))>0)
						thread_str_error("thread_manager ERROR,pthread_create call Failed",2,err);
					add++;
					pthread_mutex_lock(&ptr->lock);
					++(ptr->thread_alive);
					pthread_mutex_unlock(&ptr->lock);
				}
			}
		}
		//缩减条件
		if(busy * 2 < alive - busy && alive - _DEFCODE >= ptr->thread_min){
			pthread_mutex_lock(&ptr->lock);
			ptr->thread_exitcode = _DEFCODE;
			pthread_mutex_unlock(&ptr->lock);
			for(int i=0;i<_DEFCODE;i++)
				pthread_cond_signal(&ptr->not_empty);
		}
		printf("MANAGER PRINT SERVER STATE INFO:\n[Thread_MAX]:%d\n[Thread_MIN]:%d\n[Thread_ALIVE]:%d\n[Thread_BUSY]:%d\n[Thread_Idel]:%d\n[Container_CUR]:%d\n[Busy/Alive]:%.2f%%\n[Alive/Max]:%.2f%%\n",ptr->thread_max,ptr->thread_min,alive,busy,alive-busy,cur,(double)busy/alive * 100,(double)alive / (ptr->thread_max) * 100);
		sleep(_TIMEOUT);
	}
	pthread_exit(0);
}


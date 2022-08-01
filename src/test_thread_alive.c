/*###################################################################
# File Name: test_thread_alive.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sat 30 Jul 2022 11:54:52 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

int test_thread_alive(pthread_t tid)
{
	pthread_kill(tid,0);
	if(errno == ESRCH)
		return _FALSE;
	else 
		return _TRUE;
}

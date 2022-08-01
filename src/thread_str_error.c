/*###################################################################
# File Name: thread_str_error.c
# Author: Guo Weiye
# mail: guoweiyelai@qq.com
# Created Time: Sat 30 Jul 2022 10:40:53 AM CST
#=============================================================*/
#include<Thread_pool_server.h>

void thread_str_error(const char* errstr,int exitcode,int flag)
{
	if(!flag){
		perror(errstr);
		exit(exitcode);
	}
	printf("%s:%s\n",errstr,strerror(flag));
	exit(exitcode);
}

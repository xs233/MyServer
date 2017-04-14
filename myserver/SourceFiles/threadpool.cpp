#include<pthread.h>
#include "threadpool.h"

CMyThreadPool::CMyThreadPool()
{

}

CMyThreadPool::~CMyThreadPool()
{

}

void* CMyThreadPool::threadWorker(void* arg)
{

	return NULL;
}

bool CMyThreadPool::threadpool_init(int nMaxthread,int nMaxWait)
{
	for (int i = 0;i < nMaxthread;++i)
	{
		if(pthread_create(NULL,NULL,threadWorker,NULL))
			return false;
	}
	return true;
}

bool CMyThreadPool::threadpool_add(WorkMsg* pMsg)
{
	
	return true;
}

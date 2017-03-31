#ifndef __CEPOLL_H__
#define __CEPOLL_H__
#include<sys/epoll.h>

class CMyEpoll
{
	static const int SRV_PORT = 6666;
	static const int CONN_MAX = 10;
	int m_nfd;	
	int m_nepfd;
	epoll_event m_evs[CONN_MAX];
	
	bool myepoll_work();

public:
	CMyEpoll();
	~CMyEpoll();
	bool myepoll_init();
	bool myepoll_process();
	
};

#endif

#ifndef __CEPOLL_H__
#define __CEPOLL_H__

class CEpoll
{
	int m_nfd;	

public:
	CEpoll();
	~CEpoll();
	bool epoll_init();
};

#endif

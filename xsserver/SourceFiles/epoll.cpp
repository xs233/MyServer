#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include"../HeaderFiles/epoll.h"

CMyEpoll::CMyEpoll() : m_nfd(0),m_nepfd(0)
{
	memset(m_evs,'\0',CONN_MAX * sizeof(epoll_event));
}

CMyEpoll::~CMyEpoll()
{

}

bool CMyEpoll::myepoll_init()
{
	m_nfd = socket(PF_INET,SOCK_STREAM,0);
	if (m_nfd < 0)
	{
		perror("Create socket failed");
		return false;
	}

	sockaddr_in addr = {0};
	socklen_t addrlen = sizeof(addr);
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	addr.sin_port		= htons(SRV_PORT);
	if (bind(m_nfd,(struct sockaddr*)&addr,addrlen))
	{
		perror("Bind failed");
		close(m_nfd);
		return false;
	}

	if (listen(m_nfd,100))
	{
		perror("Listen failed");
		return false;
	}
	return true;
}

bool CMyEpoll::myepoll_process()
{
	m_nepfd = epoll_create(1);
	if (m_nepfd < 0)
	{
		perror("Create epfd failed");
		close(m_nfd);
		return false;
	}

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = m_nfd;
	epoll_ctl(m_nepfd,EPOLL_CTL_ADD,m_nfd,&ev);
	printf("Server start OK...\n");	
	
	int nRet = 0;
	while(1)
	{
		nRet = epoll_wait(m_nepfd,m_evs,CONN_MAX,-1);
		printf("epoll_wait return\n");
		if (nRet < 0)
		{
			perror("epoll_wait failed");
			close(m_nfd);
			return false;
		} else{
			
		}
	}
	return true;
}

bool CMyEpoll::myepoll_work(int nCnt)
{
	
	return true;
}

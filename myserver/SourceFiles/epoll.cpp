#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include"../HeaderFiles/epoll.h"
#include"../HeaderFiles/MyUser.h"

CMyEpoll::CMyEpoll() : m_nfd(0),m_nepfd(0)
{
	memset(m_evs,'\0',CONN_MAX * sizeof(epoll_event));
}

CMyEpoll::~CMyEpoll()
{
	 m_mapuser.clear();
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
	printf("test\n");
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
		} 
		else
		{
			myepoll_work(nRet);
		}
	}
	return true;
}

bool CMyEpoll::myepoll_work(int nCnt)
{
	for (int i=0; i<nCnt; ++i)
	{
		if (m_nfd == m_evs[i].data.fd)
		{
			printf("New User Connect...\n");
			sockaddr_in useraddr = {0};
			socklen_t nLen = sizeof(sockaddr_in);
			int nUserFd = accept(m_nfd,(struct sockaddr*)&useraddr,&nLen);
			if (nUserFd < 0)
			{
				perror("accept new connect failed!");
				continue;
			}
			else
			{
				epoll_event ev = {0};
				ev.events = EPOLLIN;
				ev.data.fd = nUserFd;
				epoll_ctl(m_nepfd,EPOLL_CTL_ADD,nUserFd,&ev);
				m_mapuser.insert(std::make_pair(nUserFd,CMyUser(nUserFd,&useraddr)));
				printf("New User from %s:%d\n",inet_ntoa(useraddr.sin_addr),ntohs(useraddr.sin_port));
				write(nUserFd,"welcome",7);
			}
		}
		else
		{
			printf("client message process\n");
			char szRecvBuf[100] = {0};
			int nRet = read(m_evs[i].data.fd,szRecvBuf,100);
			if (nRet < 0)
			{
				perror("Recv msg failed!");
				continue;
			}
			else if (0 == nRet)
			{
				std::map<int,CMyUser>::iterator it = m_mapuser.find(m_evs[i].data.fd);
				if (m_mapuser.end() == it)
				{
					perror("unknown error and lost fd!");
					continue;
				}
				printf("[%s:%d] Disconnect...\n",inet_ntoa((it->second).GetSockaddr()->sin_addr),ntohs((it->second).GetSockaddr()->sin_port));
				epoll_ctl(m_nepfd,EPOLL_CTL_DEL,m_evs[i].data.fd,m_evs + i);
				m_mapuser.erase(it);
			}
			else
			{
				std::map<int,CMyUser>::iterator it = m_mapuser.find(m_evs[i].data.fd);
				if (m_mapuser.end() == it)
				{
					perror("unknown error and lost fd!");
					continue;
				}
				printf("Recv from [%s:%d] : %s\n",inet_ntoa((it->second).GetSockaddr()->sin_addr),ntohs((it->second).GetSockaddr()->sin_port),szRecvBuf);
			}
		}
	}
	return true;
}

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"../HeaderFiles/epoll.h"

CEpoll::CEpoll() : m_nfd(0)
{

}

CEpoll::~CEpoll()
{

}

bool CEpoll::epoll_init()
{
	return false;
}

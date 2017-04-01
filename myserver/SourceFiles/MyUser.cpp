#include<string.h>
#include "../HeaderFiles/MyUser.h"

CMyUser::CMyUser() : m_nuserfd(0),m_pAddr(0)
{

}

CMyUser::CMyUser(int nfd,sockaddr_in* pAddr) : m_nuserfd(nfd)
{
	m_pAddr = new sockaddr_in;
	memcpy(m_pAddr,pAddr,sizeof(sockaddr_in));
} 

CMyUser::CMyUser(const CMyUser& myuser)
{
	m_nuserfd = myuser.m_nuserfd;
	m_pAddr = new sockaddr_in;
	memcpy(m_pAddr,myuser.m_pAddr,sizeof(sockaddr_in));
}

CMyUser::~CMyUser()
{
	if (m_pAddr)
	{
		delete m_pAddr;
		m_pAddr = nullptr;
	}
}

sockaddr_in* CMyUser::GetSockaddr()
{
	return m_pAddr;
}

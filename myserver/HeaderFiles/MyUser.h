#ifndef __CMYUSER_H__
#define __CMYUSER_H__
#include<arpa/inet.h>

class CMyUser
{
	int m_nuserfd;
	sockaddr_in* m_pAddr;

public:
	CMyUser();
	CMyUser(int nfd,sockaddr_in* pAddr);
	CMyUser(const CMyUser& myuser);
	~CMyUser();
	sockaddr_in* GetSockaddr();
};

#endif

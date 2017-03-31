#ifndef __CMYUSER_H__
#define __CMYUSER_H__
 
class CMyUser
{
	int m_nuserfd;
	sockaddr_in m_addr;

public:
	CMyUser();
	~CMyUser();

};

#endif

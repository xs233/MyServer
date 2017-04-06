#ifndef __CMYUSER_H__
#define __CMYUSER_H__
#include<arpa/inet.h>
#include<map>

class CMyUser
{
	std::map<int,sockaddr_in> m_mapuser;		

public:
	CMyUser();
	~CMyUser();
	bool insert(int,sockaddr_in);
	bool erase(int);
	bool data_process(int,char*);
	void exit();
	int get_size();
};

#endif

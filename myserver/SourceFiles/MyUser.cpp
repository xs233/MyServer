#include<string.h>
#include "../HeaderFiles/MyUser.h"

CMyUser::CMyUser()
{

}

CMyUser::~CMyUser()
{
	m_mapuser.clear();
}

bool CMyUser::insert(int nFd,sockaddr_in addr)
{
	std::pair<std::map<int,sockaddr_in>::iterator,bool> retpair = m_mapuser.insert(std::make_pair(nFd,addr));
	if (!retpair.second)
	{
		printf("insert %d fd failed!\n",nFd);
		return false;
	}
	return true;
}

bool CMyUser::erase(int nFd)
{
	std::map<int,sockaddr_in>::iterator it = m_mapuser.find(nFd);
	if (m_mapuser.end() == it)
	{
		printf("unknown error and lost %d fd",nFd);
		return false;
	}
	printf("Disconnect :[%s:%d]\n",inet_ntoa((it->second).sin_addr),ntohs((it->second).sin_port));
	m_mapuser.erase(it);
	return true;
}

bool CMyUser::data_process(int nFd, char* pMsg)
{
	std::map<int,sockaddr_in>::iterator it = m_mapuser.find(nFd);
	if (m_mapuser.end() == it)
	{
		printf("unknown error and lost %d fd",nFd);
		return false;
	}
	printf("Recv from [%s:%d] : %s\n",inet_ntoa((it->second).sin_addr),ntohs((it->second).sin_port),pMsg);
	return true;
}

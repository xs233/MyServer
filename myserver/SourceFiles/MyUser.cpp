#include<string.h>
#include<unistd.h>
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
	return true;
}

void CMyUser::exit()
{
	for (std::map<int,sockaddr_in>::iterator it = m_mapuser.begin();it != m_mapuser.end();++it)
		close(it->first);
}

int CMyUser::get_size()
{
	return m_mapuser.size();
}

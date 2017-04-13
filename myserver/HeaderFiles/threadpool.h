#include<pthread.h>

typedef struct workMsg{
	int nFd;
	char* pBuf;
} WorkMsg;

class CMyThreadPool
{

public:
	CMyThreadPool();
	~CMyThreadPool();
	bool threadpool_init(int nMaxthread,int nMaxWait);
	bool threadpool_add(WorkMsg* pMsg);
};

#include<unistd.h>
#include<fcntl.h>
#include "HeaderFiles/epoll.h"

int main()
{
	/*int nFFD = open("myserver.log",O_RDWR | O_CREAT | O_TRUNC,0700);
	if (-1 == nFFD)
	{
		perror("open myserver.log failed!");
		return -1;
	}

	if (-1 == dup2(nFFD,STDOUT_FILENO))
	{
		perror("dup2 failed");
		close(nFFD);
		return -1;
	}
	write(STDOUT_FILENO,"BBB\n",5);
	printf("AAA\n"); */
	//dup2(nFFD,STDERR_FILENO);

	//FILE* fp = freopen("myserver.log","wb",stdout);
	CMyEpoll AAA;
	AAA.myepoll_init();
	AAA.myepoll_process();
	//fclose(fp);
	return 0;
}

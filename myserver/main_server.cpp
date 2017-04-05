#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include "HeaderFiles/epoll.h"

#define FILENAME "../README.md"
typedef struct tagMsg {
	long mtype;
	char szBuf[100];
}mymsgbuf;

int init_msg()
{
	key_t key = ftok(FILENAME,1);
	if (-1 == key)
	{
		perror("ftok failed");
		return -1;
	}
	printf("key:%#x\n",key);
	
	int msqid = msgget(key,IPC_CREAT | IPC_EXCL | 0666);
	if (-1 == msqid)
	{
		if (EEXIST == errno)
		{
			printf("msgque has existed\n");
			if (0 != msgctl(msgget(key,IPC_CREAT | 0666),IPC_RMID,NULL))
			{
				perror("delete msg queue failed");
				return -1;
			}
			printf("delete the msgque\n");
			msqid = msgget(key,IPC_CREAT | 0666);
			if (-1 == msqid)
			{
				perror("msgget2 failed");
				return -1;
			}
			printf("Re-create the msgque\n");
		}
		else
		{
			perror("msgget failed");
			return -1;
		}
	}
	printf("msqid:%d\n",msqid);
	return msqid;
}

void* threadMsg(void* arg)
{
	int nMsqid = init_msg();
	if (-1 == nMsqid)
		return NULL;
	while (1)
	{
		mymsgbuf stMsg;
		stMsg.mtype = 1;
		memset(stMsg.szBuf,'\0',100);
		int nRet = msgrcv(nMsqid,&stMsg,100,1,0);
		if (-1 == nRet)
		{
			perror("msgrcv failed");
			return NULL;
		}
		printf("msgrcv:%s\n",stMsg.szBuf);
		if (strstr(stMsg.szBuf,"myserver q"))
			return NULL;
	}	
	return NULL;
}

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
	write(STDOUT_FILENO,"BBB\n",5);*/
	//printf("AAA\n");
	//dup2(nFFD,STDERR_FILENO);

	//FILE* fp = freopen("myserver.log","wb",stdout);
	
	pthread_t tid;
	pthread_create(&tid,NULL,threadMsg,NULL);
	CMyEpoll AAA;
	AAA.myepoll_init();
	AAA.myepoll_process();
	//fclose(fp);
	return 0;
}

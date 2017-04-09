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
#include<sys/wait.h>
#include "HeaderFiles/epoll.h"

#define FILENAME "../README.md"
typedef struct tagMsg {
	long mtype;
	char szBuf[100];
}mymsgbuf;
CMyEpoll AAA;

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

bool Msgsnd(int msqid,const void* msgp,size_t msgsz,int msgflg)
{
	if (-1 == msgsnd(msqid,msgp,msgsz,msgflg))
	{
		perror("msgsnd failed");
		return false;
	}
	return true;
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
		printf("msgrcv:%s",stMsg.szBuf);
		if (strstr(stMsg.szBuf,"myserver -q"))
		{
			stMsg.mtype = 2;
			memset(stMsg.szBuf,'\0',100);
			strcpy(stMsg.szBuf,"try to stop myserver...");
			if (!Msgsnd(nMsqid,&stMsg,strlen(stMsg.szBuf),0))
				return NULL;
			AAA.myepoll_end();				
			return NULL;
		}
		else if (strstr(stMsg.szBuf,"myserver -n"))
		{
			stMsg.mtype = 2;
			memset(stMsg.szBuf,'\0',100);
			sprintf(stMsg.szBuf,"current user num:%d...",AAA.get_user_num());
			if (!Msgsnd(nMsqid,&stMsg,strlen(stMsg.szBuf),0))
				return NULL;
		}
		else
		{
			stMsg.mtype = 2;
			memset(stMsg.szBuf,'\0',100);
			strcpy(stMsg.szBuf,"Invalid operation");
			if (!Msgsnd(nMsqid,&stMsg,strlen(stMsg.szBuf),0))
				return NULL;
		}
	}	
	return NULL;
}

int main()
{
	pid_t pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork1 failed");
		return -1;
	}
	else if (0 == pid1)
	{
		pid_t pid2 = fork();
		if (pid2 < 0)
		{
			perror("fork2 failed");
			return -2;
		}
		else if (0 == pid2)
		{
			//usleep(100);
			printf("create dameon process...\n");
			pthread_t tid;
			pthread_create(&tid,NULL,threadMsg,NULL);
			AAA.myepoll_init();
			AAA.myepoll_process();
		}
		wait(NULL);
		printf("know server end:%d\n",getpid());
	}
	wait(NULL);
	printf("main end:%d\n",getpid());
	return 0;
}

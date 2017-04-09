#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

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
	
	int msqid = msgget(key,IPC_CREAT | 0666);
	if (-1 == msqid)
	{
		perror("msgget failed");
		return -1;
	}
	printf("msqid:%d\n",msqid);
	return msqid;
}

bool snd_msg(int nMsqid,char* pLine)
{
	mymsgbuf stMsg;
	stMsg.mtype = 1;
	strcpy(stMsg.szBuf,pLine);
	int nRet = msgsnd(nMsqid,&stMsg,strlen(stMsg.szBuf),IPC_NOWAIT);
	if (-1 == nRet)
	{
		perror("msgsnd failed");
		return false;
	}
	stMsg.mtype = 2;
	memset(stMsg.szBuf,'\0',100);
	nRet = msgrcv(nMsqid,&stMsg,100,2,0);
	if (-1 == nRet)
	{
		perror("msgrcv failed");
		return false;
	}
	printf("rcv:%s\n",stMsg.szBuf);
	return true;
}

int main()
{
	char* pLine = NULL;
	size_t len = 0;
	int nMsqid = init_msg();
	if (-1 == nMsqid)
		return -1;
	while (-1 != getline(&pLine,&len,stdin))
	{
		if (strstr(pLine,"quit"))
			break;
		if (!snd_msg(nMsqid,pLine))
			break;
	}
	if (pLine)
		free(pLine);
	return 0;
}

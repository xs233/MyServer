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
	char* mtext;	
}msgbuf;

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
	msgbuf stMsg;
	stMsg.mtype = 1;
	stMsg.mtext = (char*)malloc(strlen(pLine) + 1);
	strcpy(stMsg.mtext,pLine);
	int nRet = msgsnd(nMsqid,&stMsg,strlen(stMsg.mtext),IPC_NOWAIT);
	if (-1 == nRet)
	{
		perror("msgsnd failed");
		return false;
	}
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
		if (!strcmp(pLine,"myserver q"))
			break;
		if (!snd_msg(nMsqid,pLine))
			break;
	}
	if (pLine)
		free(pLine);
	return 0;
}

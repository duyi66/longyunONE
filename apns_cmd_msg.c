#include <syslog.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <nebula.h>


#ifdef CONFIG_SUPPORT_USB_4G_SQLITE_APNS

#include "apns_cmd_msg.h"

static key_t	gApnsCmdMsgKey = 0xEEFFCCDD;
static int		gApnsCmdSendQid;

int APNS_CMD_InitMsg(void)
{
	printf("APNS_CMD_InitMsg进入成功\n");
	APNS_CMD_MSG msg;
	int 		i = 0;
	int         ret    = 0;
	
	//gApnsCmdMsgKey  = ftok(".", MSG_CMD_REQ);
	//if (gApnsCmdMsgKey == -1)
	//{
	//	printf("APNS_CMD_InitMsg ftok err  %d\n", gApnsCmdMsgKey);
    //   return -1;
	//}
	
	gApnsCmdSendQid  = msgget(gApnsCmdMsgKey, IPC_CREAT | 0666);//创建或打开消息队列 0666代表每个人都可以读取该消息队列
	printf("gApnsCmdSendQid:%d\n",gApnsCmdSendQid);
	if (gApnsCmdSendQid == -1)
	{
		printf("APNS_InitMsg msgget err %d\n", gApnsCmdSendQid);        
        return -1;
	}
	
	for (i = 0; i < 1000; i++)
	{
		ret = msgrcv(gApnsCmdSendQid, &msg, sizeof(APNS_CMD_MSG), 0, IPC_NOWAIT);//msgrcv函数用于从消息队列中读取msqid指定的消息
		printf("int msgrcv返回值ret：%d\n",ret);//ret=-1
		if (ret < 1)
		{
			break;
		}
		if (msg.msgHdr.msgType <= APNS_MSG_CMD_NONE || msg.msgHdr.msgType >= APNS_MSG_CMD_END_ENUM)
		{
			break;
		}
	}

	return 0;
}


int APNS_CMD_SendMsg(APNS_CMD_MSG *msg, int nLength, int nWait)
{
	int ret = 0;
	int i = 0;

    for (i = 0; i < 1000; i++)
    {
    ret = msgrcv(gApnsCmdSendQid, msg, nLength, 0, IPC_NOWAIT);
        if (ret < 1)
        {
            break;
        }

        if (msg->msgHdr.msgType <= APNS_MSG_CMD_NONE || msg->msgHdr.msgType >= APNS_MSG_CMD_END_ENUM)
        {
            break;
        }
    }

	if (nWait)
	{
		ret = msgsnd(gApnsCmdSendQid, msg, nLength, 0);	
	}
	else
	{
		ret = msgsnd(gApnsCmdSendQid, msg, nLength, IPC_NOWAIT);	
	}


	if (ret < 0)
	{
		for (i = 0; i < 1000; i++)
		{
			ret = msgrcv(gApnsCmdSendQid, msg, nLength, 0, IPC_NOWAIT);
			if (ret < 1)
			{
				break;
			}

			if (msg->msgHdr.msgType <= APNS_MSG_CMD_NONE || msg->msgHdr.msgType >= APNS_MSG_CMD_END_ENUM)
			{
				break;
			}
		}

	}

	return ret;
}

int  APNS_CMD_GetMsg(APNS_CMD_MSG *msg, int nLength)


{
	printf("进入APNS_CMD_GetMsg\n");
    int ret = 0;
	printf("ret=%d\n",ret);
	while(1)
	{	
		memset(msg,0,sizeof(APNS_CMD_MSG));
		//Get_ApnsMccMnc();
		printf("---------------gApnsCmdSendQid:%d\n",gApnsCmdSendQid);
		printf("Recv msg type:%d\n",msg->msgHdr.msgType);
		printf("Recv msg nMCC:%d\n",msg->msgHdr.nMCC);
		printf("Recv msg nMNC:%d\n",msg->msgHdr.nMNC);
		printf("Recv nDataCount:%d\n",msg->nDataCount);
		ret = msgrcv(gApnsCmdSendQid, msg, nLength, 0, 0);
		printf("ret=%d\n",ret);
		if (ret < 1)
		{	
			usleep(5000); 
			continue;
		}
		if (msg->msgHdr.msgType <= APNS_MSG_CMD_NONE || msg->msgHdr.msgType >= APNS_MSG_CMD_END_ENUM)
        {	
        	usleep(5000);
            continue;        
        }
		printf("Recv msg type:%d\n",msg->msgHdr.msgType);
		break;
	}
	
	return 0;
}
#endif

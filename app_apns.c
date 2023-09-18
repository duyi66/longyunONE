#include <stdio.h>  
#include <string.h>
#include <stdlib.h>  
#include <nebula.h>


#if 1 //#ifdef CONFIG_SUPPORT_USB_4G
#if 1 //#ifdef CONFIG_SUPPORT_USB_4G_SQLITE_APNS
#include <pthread.h>
#include "apns_db_api.h"
#include "apns_cmd_msg.h"

#define USB4G_APNS_DB_FILE_PATH "apns_full.db"

NB_INT APNS_GetApnsData(NB_CHAR * pszMCC, NB_CHAR * pszMNC) ;
#if 0
int main(int argc, char** argv)  
{ 
    NB_CHAR * DB_Name = argv[1];
    NB_CHAR * TabName = argv[2];
    NB_CHAR * mcc     = argv[3];
    NB_CHAR * mnc     = argv[4];

    APNS_GetApnsData(DB_Name,TabName,mcc,mnc);
 
}
#endif

void APNS_Eliminate(APNS_INFO * pApnsInfo_Elite,APNS_INFO * pApnsInfo)
{
	printf("进入APNS_Eliminate：pApnsInfo->nResultNum:%d\n",pApnsInfo->nResultNum);
    NB_CHAR * pAPN;
    NB_CHAR * pUser;
    NB_CHAR * pPasswd;
    NB_CHAR * pProto;
    NB_CHAR * pAuthType;
    NB_CHAR * pType;
    NB_CHAR * pCarrier;
    NB_CHAR  i,nrow_tmp;
	
    if(pApnsInfo->nResultNum <= 1)//已经是唯一，不需要剔除
    {
        for(i=0,nrow_tmp = 0;i<  pApnsInfo->nResultNum  ;i++)
        {
            memcpy(&pApnsInfo_Elite->data[nrow_tmp],&pApnsInfo->data[i],sizeof(APN_DATA));
        }

        pApnsInfo_Elite->nResultNum = 1;

        return ;
    }


    //做条件判断，遇到符合条件的，就拷贝到 pApnsInfo_Elite
    for(i=0,nrow_tmp = 0;i<  pApnsInfo->nResultNum  ;i++)
    {
        //pAPN       = pResult[i * ncolumn  + 0];
        //pUser      = pResult[i * ncolumn  + 1];
        //pPasswd    = pResult[i * ncolumn  + 2];
        //pProto     = pResult[i * ncolumn  + 3];
        //pAuthType  = pResult[i * ncolumn  + 4];
        pType      = &pApnsInfo->data[i].szType[0];
        pCarrier   = &pApnsInfo->data[i].szCarrier[0];

        //printf("Test: %d %s %s \r\n",i,pType,pCarrier);

        if(strstr(pType,"mms") || strstr(pType,"MMS") || strstr(pType,"wap") || strstr(pType,"WAP"))
        {
            ;//数据不符合
        }
        else if(strstr(pCarrier,"mms") || strstr(pCarrier,"MMS") || strstr(pCarrier,"wap") || strstr(pCarrier,"WAP"))
        {
            ;//数据不符合
        }
        else
        {  //数据符合  ，拷贝到 pApnsInfo_Elite
            memcpy(&pApnsInfo_Elite->data[nrow_tmp],&pApnsInfo->data[i],sizeof(APN_DATA));
            nrow_tmp++;
        }
    }

    pApnsInfo_Elite->nResultNum = nrow_tmp;
    pApnsInfo_Elite->nMCC = pApnsInfo->nMCC;
    pApnsInfo_Elite->nMNC = pApnsInfo->nMNC;

    return ;
}


void APNS_PrintfData(APNS_INFO * pApnsInfo)
{	
    APN_DATA * pData = NULL;
    int i;

    printf("———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\r\n");
    printf("|apn:            |User:           |Passwd:         |protocol:       |authtype:       |type:           |Carrier         |\r\n");

    for(i = 0;i < pApnsInfo->nResultNum;i++)
    {
        pData = &pApnsInfo->data[i];
        printf("|%16s|%16s|%16s|%16s|%16s|%16s|%16s|\r\n", \
            pData->szAPN,      \
            pData->szUser,     \
            pData->szPasswd,   \
            pData->szProto,    \
            pData->szAuthType, \
            pData->szType,     \
            pData->szCarrier   \
            );
    }
    printf("———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\r\n");
}


void APNS_PrintfData_short(APNS_INFO * pApnsInfo)
{
	
    APN_DATA * pData = NULL;
    int i;

    for(i = 0;i < pApnsInfo->nResultNum;i++)
    {
        pData = &pApnsInfo->data[i];
        printf("|%16s|%16s|%16s|%16s|%16s|%16s|%16s|\r\n", \
            pData->szAPN,      \
            pData->szUser,     \
            pData->szPasswd,   \
            pData->szProto,    \
            pData->szAuthType, \
            pData->szType,     \
            pData->szCarrier   \
            );
    }
}

#define DEFAULT_APNS_DB_NAME  __DEFAULT_RES_DIR__ "/apns_full.db"
#define DEFAULT_APNS_TABLE_NAME "Android_2016"
static NB_INT g_ApnsMsgExitFlag = 0;

NB_VOID * APNS_MSG_CMD_RecvProc(NB_VOID)
{
	
	printf("进入APNS_MSG_CMD_RecvProc\n");
    NB_INT i,ret;
    NB_INT mcc,mnc;
    APNS_INFO * pApnsInfo = NULL;
    APNS_CMD_MSG * msg = NULL;
    APN_DATA * pMsgData;
    APNS_INFO * pRetApnsInfo = NULL;
    

    APNS_CMD_InitMsg();
	printf("退出APNS_CMD_InitMsg\n");
    msg = malloc(sizeof(APNS_CMD_MSG));
	//printf("msg%u\n",&msg);
    pRetApnsInfo = malloc(sizeof(APNS_INFO));
	//printf("pRetApnsInfo%u\n",&pRetApnsInfo);
    if(msg == NULL || pRetApnsInfo == NULL)
    {
        printf("APNS malloc error, return\n");
        return NULL;
    }
    

    //printf( "APNS_MSG_CMD_RecvProc In \n" );
	printf("数据g_ApnsMsgExitFlag:%d\n",g_ApnsMsgExitFlag);
    while(g_ApnsMsgExitFlag==0)
    {	
        APNS_CMD_GetMsg(msg, sizeof(APNS_CMD_MSG));//在消息队列中获取Mcc和Mnc数据。
        if(msg->msgHdr.msgType  == APNS_MSG_CMD_QUERY_INFO)
        {
            pApnsInfo = NULL;
			//printf("pApnsInfo:%u\n",pApnsInfo);
            mcc = msg->msgHdr.nMCC;
            mnc = msg->msgHdr.nMNC;
            ret = APNS_DB_GetInfoByMmcMncString(mcc,mnc,&pApnsInfo);
            printf("Query info ret:%d\n",ret);
            if(ret == NB_R_SUCCESS)
            {
                if(pApnsInfo == NULL)
                {
                    printf("something wrong!\n");
                    continue;
                }
                memset(pRetApnsInfo,0,sizeof(APNS_INFO));
                APNS_Eliminate(pRetApnsInfo,pApnsInfo);
                APNS_PrintfData(pRetApnsInfo);
                memset(msg,0,sizeof(APNS_CMD_MSG));
                msg->msgHdr.msgType = APNS_MSG_CMD_REPLY_DATA;
                msg->msgHdr.nMCC = mcc;
                msg->msgHdr.nMNC = mnc;
                msg->nDataCount = pRetApnsInfo->nResultNum;
                for(i=0;i<pRetApnsInfo->nResultNum;i++)
                {
                    pMsgData = (APN_DATA *)&msg->data[i];
                    memcpy(pMsgData,&pRetApnsInfo->data[i],sizeof(APN_DATA));
                }
                ret = APNS_CMD_SendMsg(msg, sizeof(APNS_CMD_MSG),0);
                printf("Send msg ret:%d\n",ret);
            }
        }
        else if(msg->msgHdr.msgType  == APNS_MSG_CMD_EXIT)
        {
            g_ApnsMsgExitFlag = 1;
            printf("Exit program\n");
        }
    }
    printf("APNS_MSG_CMD_RecvProc Exit\n");
    if(msg)
    {
        free(msg);
        msg = NULL;
    }
    if(pRetApnsInfo)
    {
        free(pRetApnsInfo);
        pRetApnsInfo = NULL;
    }
    return NULL;
}

NB_INT main(NB_VOID)
{
    NB_INT ret=-1;
    pthread_t tApnsCmd;//线程声明
    ret = APNS_DB_Init(USB4G_APNS_DB_FILE_PATH,DEFAULT_APNS_TABLE_NAME);
	printf("--------%d\n",ret);
    if(ret == NB_R_FAILURE)
    {
        printf("ERROR: Init apns db:%s failed\n",USB4G_APNS_DB_FILE_PATH);
        return NB_R_FAILURE;
    }
    pthread_create(&tApnsCmd, 0, (NB_VOID*)APNS_MSG_CMD_RecvProc, NULL);//创建线程，从APNS_MSG_CMD_RecvProc函数开始运行
    ret = pthread_join(tApnsCmd, NULL);
    if (ret != 0)
    {
        printf("tApnsCmd: pthread_join failed\n");
    }
    APNS_DB_Release();
    return ret;
}
#endif
#endif

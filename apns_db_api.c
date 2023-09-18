#include <stdio.h>  
#include <string.h>
#include <stdlib.h> 
#include <nebula.h>
#include "apns_cmd_msg.h"
#ifdef CONFIG_SUPPORT_USB_4G
#ifdef CONFIG_SUPPORT_USB_4G_SQLITE_APNS
#include "apns_db_api.h"

static APNS_DB * g_pApnsDBPtr = NULL;
static APNS_INFO * g_pApnsInfo = NULL;

#define DEBUG(format,...)  
//#define DEBUG(format,...)  printf(format,##__VA_ARGS__);  



/*
1.通过mcc mnc 输出是不是唯一的，如果不是唯一
2.就判断type，看  有没有wap mmc  ，有就剔除，如果还不是唯一 
3.就判断 carrier，看  有没有wap mmc  ，如果还不是唯一 
4.就判断 所有带mms 就都不要
*/


int APNS_DB_Check(NB_CHAR **pResultn)
{


	return 0;
}

void APNS_DB_Eliminate(NB_CHAR **pResult,NB_INT *nrow,NB_INT ncolumn)
{
    //NB_CHAR * pAPN;
    //NB_CHAR * pUser;
    //NB_CHAR * pPasswd;
    //NB_CHAR * pProto;
    //NB_CHAR * pAuthType;
    NB_CHAR * pType;
    NB_CHAR * pCarrier;
    NB_UINT  i,nrow_tmp;
    NB_CHAR **pResult_tmp;

    if(*nrow <= 1)//已经是唯一，不需要剔除
    {
        return ;
    }

    pResult_tmp = malloc((*nrow * ncolumn) * sizeof(void *));

    //做条件判断，遇到符合条件的，就拷贝到data_Tmp,并计数
    for(i=0,nrow_tmp = 0;i<  (*nrow + 1);i++)
    {
        //pAPN       = pResult[i * ncolumn  + 0];
        //pUser      = pResult[i * ncolumn  + 1];
        //pPasswd    = pResult[i * ncolumn  + 2];
        //pProto     = pResult[i * ncolumn  + 3];
        //pAuthType  = pResult[i * ncolumn  + 4];
        pType      = pResult[i * ncolumn  + 5];
        pCarrier   = pResult[i * ncolumn  + 6]; 

        if(strstr(pType,"mms") || strstr(pType,"MMS") || strstr(pType,"wap") || strstr(pType,"WAP"))
        {
            ;//数据不符合
        }
        else if(strstr(pCarrier,"mms") || strstr(pCarrier,"MMS") || strstr(pCarrier,"wap") || strstr(pCarrier,"WAP"))
        {
            ;//数据不符合
        }
        else
        {  //数据符合  ，拷贝到data_Tmp,并且nrow_tmp++
            pResult_tmp[nrow_tmp * ncolumn + 0]   = pResult[i * ncolumn  + 0];
            pResult_tmp[nrow_tmp * ncolumn + 1]   = pResult[i * ncolumn  + 1]; 
            pResult_tmp[nrow_tmp * ncolumn + 2]   = pResult[i * ncolumn  + 2]; 
            pResult_tmp[nrow_tmp * ncolumn + 3]   = pResult[i * ncolumn  + 3]; 
            pResult_tmp[nrow_tmp * ncolumn + 4]   = pResult[i * ncolumn  + 4];;
            pResult_tmp[nrow_tmp * ncolumn + 5]   = pResult[i * ncolumn  + 5]; 
            pResult_tmp[nrow_tmp * ncolumn + 6]   = pResult[i * ncolumn  + 6]; 
            
            nrow_tmp++;
        }
    }

    *nrow = nrow_tmp-1;

    //数据整合，把azResult_tmp  更新到azResult
    for(i=0; i < (*nrow + 1);i++)
    {  
        pResult[i * ncolumn  + 0]  = pResult_tmp[i * ncolumn  + 0];
        pResult[i * ncolumn  + 1]  = pResult_tmp[i * ncolumn  + 1];
        pResult[i * ncolumn  + 2]  = pResult_tmp[i * ncolumn  + 2]; 
        pResult[i * ncolumn  + 3]  = pResult_tmp[i * ncolumn  + 3];
        pResult[i * ncolumn  + 4]  = pResult_tmp[i * ncolumn  + 4];
        pResult[i * ncolumn  + 5]  = pResult_tmp[i * ncolumn  + 5];
        pResult[i * ncolumn  + 6]  = pResult_tmp[i * ncolumn  + 6];

        /*
        pAPN       = pResult[i * ncolumn  + 0];
        pUser      = pResult[i * ncolumn  + 1];
        pPasswd    = pResult[i * ncolumn  + 2];
        pProto     = pResult[i * ncolumn  + 3];
        pAuthType  = pResult[i * ncolumn  + 4];
        pType      = pResult[i * ncolumn  + 5];
        pCarrier   = pResult[i * ncolumn  + 6];
        DEBUG("|%16s|%16s|%16s|%16s|%16s|%16s|%16s|\r\n", pAPN,pUser,pPasswd,pProto,pAuthType,pType,pCarrier);*/
    }

    

    free(pResult_tmp);

    return ;
}



//返回一个句柄，
NB_INT APNS_DB_Init(NB_CHAR * pszDBName,NB_CHAR * pszTblName )
{
	printf("APNS_DB_Init,pszDBName=%s,pszTblName=%s\n",pszDBName,pszTblName);
    NB_INT ret;
    if(pszDBName == NULL || pszTblName == NULL || pszDBName[0]=='\0' || pszTblName[0] == '\0') 
    {
        printf("Invalid param\n");
        return NB_R_FAILURE;
    }
    
    if(g_pApnsDBPtr == NULL)
    {
        g_pApnsDBPtr = malloc(sizeof(APNS_DB));
        if(g_pApnsDBPtr == NULL)
        {
            printf("malloc anps db failed\n");
            return NB_R_FAILURE;
        }
    }

    if(g_pApnsInfo == NULL)
    {
        g_pApnsInfo = malloc(sizeof(APNS_INFO));
        if(g_pApnsInfo == NULL)
        {
            printf("malloc anps info failed\n");
            return NB_R_FAILURE;
        }
    }
        
    memset(g_pApnsDBPtr,0,sizeof(APNS_DB));
    memset(g_pApnsInfo,0,sizeof(APNS_INFO));
    if(!g_pApnsDBPtr||!g_pApnsInfo)
		printf("g_pApnsDBPtr不存在\n");
    g_pApnsInfo->nResultNum = 0;
    printf("%d\n",g_pApnsInfo->nResultNum);
    strcpy(g_pApnsDBPtr->szDBName,pszDBName);
    strcpy(g_pApnsDBPtr->szTblName,pszTblName);
	printf("g_pApnsDBPtr->szDBName:%s\n",g_pApnsDBPtr->szDBName);
	printf("g_pApnsDBPtr->szTblName:%s\n",g_pApnsDBPtr->szTblName);
    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件  
    ret = sqlite3_open(g_pApnsDBPtr->szDBName,&g_pApnsDBPtr->pSqliteDB);  
	printf("--------sqlite3_open:%d\n",ret);
    if(ret != SQLITE_OK)  
    {  
        printf("Open database: %s failed\r\n",g_pApnsDBPtr->szDBName); 
        APNS_DB_Release();
        return NB_R_FAILURE;
    } 
    printf("\r\nopened1111111  \" %s \" \r\n",g_pApnsDBPtr->szDBName); 
    return NB_R_SUCCESS;
}

NB_VOID APNS_DB_Release(NB_VOID)
{
    if(g_pApnsDBPtr && g_pApnsDBPtr->pSqliteDB)
    {
        sqlite3_close(g_pApnsDBPtr->pSqliteDB); //关闭数据库  
    }
    if(g_pApnsDBPtr)
    {
        free(g_pApnsDBPtr);
        g_pApnsDBPtr = NULL;
    }
    if(g_pApnsInfo)
    {
        free(g_pApnsInfo);
        g_pApnsInfo = NULL;
    }
}


/* NB_INT APNS_DB_GETMmcMnc(){
	
	
} */

//Return:  <=0  Error

NB_INT APNS_DB_GetInfoByMmcMncString(NB_INT nMCC,NB_INT nMNC,APNS_INFO **pResult)//根据MCC和MNC到的来查询其他数据放入（APN_DATA）data中
{ 
	printf("进入APNS_DB_GetInfoByMmcMncString：————\n");
    NB_INT i=0,j=0;
    NB_INT nrow=0;
    NB_INT ncolumn = 0;
    NB_CHAR *pszErrMsg =NULL;
    NB_CHAR **azResult=NULL;
    NB_CHAR szSQLCmd[256];  
    NB_CHAR szMCC[8]={0};
    NB_CHAR szMNC[8]={0};
    NB_INT ret=0;
    APN_DATA * data = NULL;
    APNS_DB * pDB = g_pApnsDBPtr;
    APNS_INFO * pInfo = g_pApnsInfo;
    if(pDB == NULL || pInfo == NULL)
    {
        printf("Invalid ptr\n");
        pInfo->nResultNum = 0;
        return NB_R_FAILURE;
    }
    g_pApnsInfo->nMCC = nMCC;
    g_pApnsInfo->nMNC = nMNC;
    snprintf(szMCC,sizeof(szMCC),"%03d",nMCC);
    snprintf(szMNC,sizeof(szMNC),"%02d",nMNC);
    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件  
    memset(szSQLCmd,0,sizeof(szSQLCmd));
    snprintf(szSQLCmd,sizeof(szSQLCmd),"select apn,user,password,protocol,authtype,type,carrier from '%s' where mcc=%s and mnc=%s",
            pDB->szTblName,szMCC,szMNC);

    /* 查询数据 */
    printf("SQL CMD:\"%s\"\r\n",szSQLCmd);
    ret = sqlite3_get_table( pDB->pSqliteDB , szSQLCmd , &azResult , &nrow , &ncolumn , &pszErrMsg );
    if(ret != SQLITE_OK)
    {
        printf("Get data failed\n");
        pInfo->nResultNum = 0;
        ret = NB_R_FAILURE;
        goto out;
    }
    printf("Search Result: ErrMsg:%s     nrow=%d ncolumn=%d\n",pszErrMsg,nrow,ncolumn);

    if(nrow==0)
    {
        pInfo->nResultNum = 0;
        ret = NB_R_SUCCESS;
        goto out;
    }
  
    for(i=0;i<nrow+1;i++)
    {
        for(j=0;j<ncolumn;j++)
        {
            DEBUG("%16s ",azResult[i*ncolumn+j]);
        }

        DEBUG("\r\n");
    }


    for(i=1,j=0;i<nrow+1;j++,i++)
    {
        data = &pInfo->data[j];
        //超出
        if(j  >= MAX_APNS_DATA_NUM)
        {
            pInfo->nResultNum = MAX_APNS_DATA_NUM;
            ret = NB_R_SUCCESS;
            goto out;
        }


        if(azResult[i*ncolumn+0])
            strcpy(data->szAPN, azResult[i*ncolumn+0]);  

        if(azResult[i*ncolumn+1])
            strcpy(data->szUser, azResult[i*ncolumn+1]);

        if(azResult[i*ncolumn+2])
            strcpy(data->szPasswd, azResult[i*ncolumn+2]);  

        if(azResult[i*ncolumn+3])
            strcpy(data->szProto, azResult[i*ncolumn+3]);  

        if(azResult[i*ncolumn+4])
            strcpy(data->szAuthType, azResult[i*ncolumn+4]);  

        if(azResult[i*ncolumn+5])
            strcpy(data->szType, azResult[i*ncolumn+5]);  

        if(azResult[i*ncolumn+6])
            strcpy(data->szCarrier, azResult[i*ncolumn+6]);  
            
    }

    ret = NB_R_SUCCESS;
    
    pInfo->nResultNum = j;

    out: 

    
    sqlite3_free_table(azResult);
    if(pszErrMsg)
    {
        sqlite3_free(pszErrMsg);
    }
    *pResult = g_pApnsInfo;
    
    
    return ret;  
}

/* int Get_ApnsMccMnc(){
	sqlite3 *db = NULL;
	char *err = NULL;
	int ret,i,j;
	char *sql;
	int row,col;
	NB_CHAR **result;
	ret= sqlite3_open("apns_full.db",&db);
	if(ret !=SQLITE_OK){
		printf("open error\n");
		return -1;
	}
	sql = "select mcc,mnc from android_2016";
	sqlite3_get_table(db,sql,&result,&row,&col,&err);
	if(err){
		printf("err:%s\n",err);
		sqlite3_close(db);
		return -1;
	}
	struct _APNS_MSG_HDR_ mydata[row];
	for(i=1;i<row;i++){
		printf("mcc=%s______mnc=%s\n",result[i*col+0],result[i*col+1]);
		mydata[i].nMCC=atoi(result[i*col+0]);
		mydata[i].nMNC=atoi(result[i*col+1]);
	}
	for(i=1;i<row;i++){
		printf("data：mcc=%d______mnc=%d\n",mydata[i].nMCC,mydata[i].nMNC);
	}
	sqlite3_free_table(result);
	sqlite3_close(db);
	return 0;
} */



/*
CREATE TABLE "apns-full-conf_Android_2006" (
  "carrier" TEXT(255),
  "carrier_id" TEXT(255),
  "mcc" TEXT(255),
  "mnc" TEXT(255),
  "apn" TEXT(255),
  "user" TEXT(255),
  "password" TEXT(255),
  "proxy" TEXT(255),
  "port" TEXT(255),
  "mmsc" TEXT(255),
  "mmsproxy" TEXT(255),
  "mmsport" TEXT(255),
  "type" TEXT(255),
  "mtu" TEXT(255),
  "protocol" TEXT(255),
  "authtype" TEXT(255)
) */

#endif
#endif

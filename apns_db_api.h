#ifndef  __APNS_DB_API__H__
#define  __APNS_DB_API__H__

#include "sqlite3.h"

#define MAX_APNS_DATA_NUM      6

#define APN_NAME_MAX_SIZE      128
#define APN_USER_MAX_SIZE      128
#define APN_PASSWD_MAX_SIZE    128
#define APN_PROTO_MAX_SIZE     128
#define APN_AUTHTYPE_MAX_SIZE  128
#define APN_TYPE_MAX_SIZE      128
#define APN_CARRIER_MAX_SIZE   128

typedef struct _APN_DATA_
{
    NB_CHAR szAPN[APN_NAME_MAX_SIZE];
    NB_CHAR szUser[APN_USER_MAX_SIZE];
    NB_CHAR szPasswd[APN_PASSWD_MAX_SIZE];
    NB_CHAR szProto[APN_PROTO_MAX_SIZE];
    NB_CHAR szAuthType[APN_AUTHTYPE_MAX_SIZE];
    NB_CHAR szType[APN_TYPE_MAX_SIZE];
    NB_CHAR szCarrier[APN_CARRIER_MAX_SIZE];
} APN_DATA;

typedef struct _APNS_INFO_
{
    NB_INT  nMCC;
    NB_INT  nMNC;
    APN_DATA data[MAX_APNS_DATA_NUM];
    NB_INT nResultNum;
} APNS_INFO;

typedef struct _APNS_DB_ {   
    NB_CHAR szDBName[128];
    NB_CHAR szTblName[128];
    sqlite3 * pSqliteDB;
}APNS_DB;

NB_INT APNS_DB_Init(NB_CHAR * pszDBName,NB_CHAR * pszTblName );
NB_INT APNS_DB_GetInfoByMmcMncString(NB_INT nMCC,NB_INT nMNC,APNS_INFO **pResult);
NB_VOID APNS_DB_Release(NB_VOID);



#endif

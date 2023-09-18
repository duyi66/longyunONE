#ifndef __APNS_CMD_MSG_H__
#define __APNS_CMD_MSG_H__

#define  APNS_MSG_CMD_REQ				    'n'
#define  APNS_MSG_CMD_PROJ_ID  			    81

#define MAX_APNS_MSG_DATA_NUM      10

#define APN_MSG_NAME_MAX_SIZE      128
#define APN_MSG_USER_MAX_SIZE      64
#define APN_MSG_PASSWD_MAX_SIZE    64
#define APN_MSG_PROTO_MAX_SIZE     64
#define APN_MSG_AUTHTYPE_MAX_SIZE  64
#define APN_MSG_TYPE_MAX_SIZE      64
#define APN_MSG_CARRIER_MAX_SIZE   128

typedef struct _APNS_MSG_HDR_ {
    NB_INT msgType;
    NB_INT  nMCC;
    NB_INT  nMNC;
}APNS_MSG_HDR;

typedef struct _APN_MSG_DATA_
{
    NB_CHAR szAPN[APN_MSG_NAME_MAX_SIZE];
    NB_CHAR szUser[APN_MSG_USER_MAX_SIZE];
    NB_CHAR szPasswd[APN_MSG_PASSWD_MAX_SIZE];
    NB_CHAR szProto[APN_MSG_PROTO_MAX_SIZE];
    NB_CHAR szAuthType[APN_MSG_AUTHTYPE_MAX_SIZE];
    NB_CHAR szType[APN_MSG_TYPE_MAX_SIZE];
    NB_CHAR szCarrier[APN_MSG_CARRIER_MAX_SIZE];
} APN_MSG_DATA;
typedef struct _APNS_CMD_MSG_
{
    APNS_MSG_HDR msgHdr;
    NB_INT nDataCount;
    APN_MSG_DATA data[MAX_APNS_MSG_DATA_NUM];    
} APNS_CMD_MSG;

typedef enum {
	APNS_MSG_CMD_NONE =0,
	APNS_MSG_CMD_QUERY_INFO=1,
	APNS_MSG_CMD_REPLY_DATA,
	APNS_MSG_CMD_EXIT,
	APNS_MSG_CMD_END_ENUM,
}MSG_CMD_ENUM;
    

int APNS_CMD_InitMsg(void);
int APNS_CMD_SendMsg(APNS_CMD_MSG *msg, int nLength, int nWait);
int APNS_CMD_GetMsg(APNS_CMD_MSG *msg, int nLength);

#endif
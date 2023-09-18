#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <nebula.h>
#include <apns_cmd_msg.h>
#include <apns_db_api.h>
//#include <mccdemo.h>
#define PACKET_SIZE 64
#define MAX_IP_ADDRESSES 3
int Get_ApnsMccMncOtherNumber(APN_DATA** mydata,NB_INT nMCC,NB_INT nMNC){
	sqlite3 *db = NULL;
	char *err = NULL;
	int ret,i,j;
	NB_CHAR szSQLCmd[256];
	int row,col;
	NB_CHAR **result;
	NB_CHAR szMCC[8]={0};
    NB_CHAR szMNC[8]={0};
	NB_CHAR tableName[12]="Android_2016";
	ret= sqlite3_open("apns_full.db",&db);
	if(ret !=SQLITE_OK){
		printf("open error\n");
		return -1;
	}
	snprintf(szMCC,sizeof(szMCC),"%03d",nMCC);
    snprintf(szMNC,sizeof(szMNC),"%02d",nMNC);
	 memset(szSQLCmd,0,sizeof(szSQLCmd));
    snprintf(szSQLCmd,sizeof(szSQLCmd),"select apn,user,password,protocol,authtype,type,carrier from '%s' where mcc=%s and mnc=%s",
           tableName,szMCC,szMNC);
	//sql = "select mcc,mnc from android_2016";
	sqlite3_get_table(db,szSQLCmd,&result,&row,&col,&err);
	if(err){
		printf("err:%s\n",err);
		sqlite3_close(db);
		return -1;
	}
/* 	for(i=1;i<=(row*col)/7;i++){
		printf("szAPN=%s  szUser=%s  szPasswd=%s  szProto=%s  szAuthType=%s  szType=%s  szCarrier=%s\n",result[i],result[i+1],result[i+2],result[i+3],result[i+4],result[i+5],result[i+6]);
	}  */ //查看是否查询到数据
	//APN_MCC_DATA* mydata=malloc(sizeof(APN_MCC_DATA)*row);
	//APN_MCC_DATA mydata[row];
	*mydata = malloc(sizeof(APN_DATA) * row);
	/* for(j=0,i=1;i<row+1;i++,j++){
		stpcpy((mydata)[j]->szAPN,"");
		stpcpy(mydata[j]->szUser,"");
		stpcpy(mydata[j]->szPasswd,"");
		stpcpy(mydata[j]->szProto,"");
		stpcpy(mydata[j]->szAuthType,"");
		stpcpy(mydata[j]->szType,"");
		stpcpy(mydata[j]->szCarrier,"");
	} */
	for(j=0,i=1;i<row+1;i++,j++){
		if(result[i*col+0])
			strcpy((*mydata)[j].szAPN,result[i*col+0]);
		if(result[i*col+1])
			strcpy((*mydata)[j].szUser,result[i*col+1]);
		if(result[i*col+2])
			strcpy((*mydata)[j].szPasswd,result[i*col+2]);
		if(result[i*col+3])
			strcpy((*mydata)[j].szProto,result[i*col+3]);
		if(result[i*col+4])
			strcpy((*mydata)[j].szAuthType,result[i*col+4]);
		if(result[i*col+5])
			strcpy((*mydata)[j].szType,result[i*col+5]);
		if(result[i*col+6])
			strcpy((*mydata)[j].szCarrier,result[i*col+6]);
	}
	
	//printf("mydata[1].szType=%s\n",mydata[1]->szType);
	printf("———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\r\n");
    printf("|apn:            |User:           |Passwd:         |protocol:       |authtype:       |type:           |Carrier         |\r\n");
	for(i = 0;i <row;i++){
		 printf("|%16s|%16s|%16s|%16s|%16s|%16s|%16s|\r\n", \
            (*mydata)[i].szAPN,      \
            (*mydata)[i].szUser,     \
            (*mydata)[i].szPasswd,   \
            (*mydata)[i].szProto,    \
            (*mydata)[i].szAuthType, \
            (*mydata)[i].szType,     \
            (*mydata)[i].szCarrier   \
            );
	}
	printf("———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\r\n");
	sqlite3_free_table(result);
	sqlite3_close(db);
	return 0;
}

/* NB_VOID ScreenData(APN_MCC_DATA* mydata,APN_MCC_DATA* afterScreenData){
	NB_INT i;
	for(i=0;i<14;i++){
		
	}
	
	
	
} */
// 计算校验和
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *) buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

// 发送ICMP回显请求包
void send_ping_request(int sockfd, struct sockaddr_in *dest_addr, int seq_num) {
    struct icmp *icmp_packet;
    char packet[PACKET_SIZE];
    memset(packet, 0, sizeof(packet));

    icmp_packet = (struct icmp *) packet;
    icmp_packet->icmp_type = ICMP_ECHO;
    icmp_packet->icmp_code = 0;
    icmp_packet->icmp_id = getpid();
    icmp_packet->icmp_seq = seq_num;
    icmp_packet->icmp_cksum = 0;
    icmp_packet->icmp_cksum = checksum(icmp_packet, PACKET_SIZE);

    // 发送请求包
    if (sendto(sockfd, packet, PACKET_SIZE, 0, (struct sockaddr *) dest_addr, sizeof(*dest_addr)) == -1) {
        perror("sendto");
        exit(-1);
    }
}

// 接收ICMP回显应答包
int receive_ping_reply(int sockfd, struct sockaddr_in *dest_addr, int seq_num) {
    char buffer[PACKET_SIZE];
    socklen_t addr_len = sizeof(*dest_addr);

    // 设置超时时间为 1 秒
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    // 设置文件描述符集合
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    // 等待套接字可读或超时
    int ready = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

    if (ready == -1) {
        perror("select");
        exit(-1);
    } else if (ready == 0) {
        printf("Request timed out.%s\n",inet_ntoa(dest_addr->sin_addr));
        return -1;  // 超时，不进行接收处理
    }

    // 收到响应，执行接收处理
    // ...

    // 接收响应包
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) dest_addr, &addr_len) == -1) {
        perror("recvfrom");
        return -1;
    }
    printf("Received ping reply from %s\n", inet_ntoa(dest_addr->sin_addr));
    return 0;
}



int PingIP(){
	int sockfd;
    struct hostent *host;
    struct sockaddr_in dest_addr;
    int seq_num = 0;
    int i, flag;
    char ip_addresses[MAX_IP_ADDRESSES][100] = {
        "39.156.66.10",
        "192.168.0.2",
        "192.168.0.3"
    };

    // 创建原始套接字
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1) {
        perror("socket");
        exit(-1);
    }

    for (i = 0; i < MAX_IP_ADDRESSES; i++) {
        // 获取目标主机信息
        host = gethostbyname(ip_addresses[i]);
        if (host == NULL) {
            fprintf(stderr, "Unknown host %s\n", ip_addresses[i]);
            continue;
        }

        // 设置目标地址结构
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = 0;
        dest_addr.sin_addr = *((struct in_addr *)host->h_addr);

        seq_num = 0;
        while (seq_num < 4) {  // 发送4个ping请求
            send_ping_request(sockfd, &dest_addr, seq_num);
            flag = receive_ping_reply(sockfd, &dest_addr, seq_num);
            seq_num++;

            printf("...........................flag=%d\n", flag);
            if (0 == flag)
                printf("可以ping通，这个地址或域名存在，正在被使用\n");
            else
                printf("不可以ping通，这个地址或域名不存在或者未被使用\n");
        }
    }

    // 关闭套接字
    close(sockfd);
    return 0;
}




int main(int argc, char *argv[])
{
	APNS_CMD_MSG msg;
	int ret=0;
	int i;
	int mcc,mnc;
	
	APN_DATA* mydata=NULL;
	//APN_DATA* afterScreenData =NULL;
	memset(&msg,0,sizeof(msg));
	msg.msgHdr.msgType = APNS_MSG_CMD_QUERY_INFO;
	msg.msgHdr.nMCC = 208;
	msg.msgHdr.nMNC = 1;
	mcc=msg.msgHdr.nMCC;
	mnc=msg.msgHdr.nMNC;
	APNS_CMD_InitMsg();
	ret = APNS_CMD_SendMsg(&msg, sizeof(APNS_CMD_MSG),0);
	printf("Send msg ret:%d\n",ret);
	Get_ApnsMccMncOtherNumber(&mydata,mcc,mnc);
	//ScreenData(&mydata,&afterScreenData);
	PingIP();
	
	free(mydata);
	return 0;
}


#pragma once
const int MAX_BUF_SIZE = 512;		//一个DNS包的最大长度
const int MAX_ID_TABLE_SIZE = 16; //ID对应表最大容量
const int TTL = 2;				//生存期最大为两秒
const int CACHE_TTL = 5;//cache生存周期为五次查询
const int DNS_HEADER_SIZE = 12; //DNS头字节数
const int DNS_PORT = 53;//DNS端口
const int MAX_CACHE_SIZE = 5;//CACHE表容量

#define BUF_SIZE 1024          /* Max buffer size */
extern SOCKADDR_IN client_s;
extern struct sockaddr_in extern_id;

const int MAX_IP_LEN = 25;
const int MAX_DOMAIN_NAME_LEN = 100;
const int MAX_RECORDS_NUM = 2000;
const int MAX_IP_COUNT = 7;//每个域名最多对应七条资源记录

struct Record
{
	char ip[MAX_IP_LEN];
	char dn[MAX_DOMAIN_NAME_LEN];
};

struct AllRecords
{
	Record record[MAX_RECORDS_NUM];
	int count;
};

struct Cache_Unit {
	char dn[MAX_DOMAIN_NAME_LEN];
	char ip[MAX_IP_COUNT][MAX_IP_LEN];//总共资源记录
	int ip_count;//资源记录数
	int ttl;//生存次数
};//缓存单元

extern int debug_level;
extern SOCKET local_sock;
extern SOCKET extern_sock;
extern SOCKADDR clntAddr;

int query();
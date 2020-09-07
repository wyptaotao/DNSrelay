#pragma once
const int MAX_BUF_SIZE = 512;		//һ��DNS������󳤶�
const int MAX_ID_TABLE_SIZE = 16; //ID��Ӧ���������
const int TTL = 105;				//���������Ϊ����
const int DNS_HEADER_SIZE = 12; //DNSͷ�ֽ���
const int DNS_PORT = 53;//DNS�˿�
const int MAX_CACHE_SIZE = 5;//CACHE������

#define BUF_SIZE 1024          /* Max buffer size */
extern SOCKADDR_IN client_s;
extern struct sockaddr_in extern_id;

const int MAX_IP_LEN = 25;
const int MAX_DOMAIN_NAME_LEN = 100;
const int MAX_RECORDS_NUM = 2000;

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
	Record inf;
	int ttl;//�������
};//���浥Ԫ

extern int debug_level;
extern SOCKET local_sock;
extern SOCKET extern_sock;
extern SOCKADDR clntAddr;

int query();
#pragma once
const int MAX_BUF_SIZE = 512;		//һ��DNS������󳤶�
const int MAX_ID_TABLE_SIZE = 16; //ID��Ӧ���������
const int TTL = 2;				//���������Ϊ����
const int CACHE_TTL = 5;//cache��������Ϊ��β�ѯ
const int DNS_HEADER_SIZE = 12; //DNSͷ�ֽ���
const int DNS_PORT = 53;//DNS�˿�
const int MAX_CACHE_SIZE = 5;//CACHE������

#define BUF_SIZE 1024          /* Max buffer size */
extern SOCKADDR_IN client_s;
extern struct sockaddr_in extern_id;

const int MAX_IP_LEN = 25;
const int MAX_DOMAIN_NAME_LEN = 100;
const int MAX_RECORDS_NUM = 2000;
const int MAX_IP_COUNT = 7;//ÿ����������Ӧ������Դ��¼

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
	char ip[MAX_IP_COUNT][MAX_IP_LEN];//�ܹ���Դ��¼
	int ip_count;//��Դ��¼��
	int ttl;//�������
};//���浥Ԫ

extern int debug_level;
extern SOCKET local_sock;
extern SOCKET extern_sock;
extern SOCKADDR clntAddr;

int query();
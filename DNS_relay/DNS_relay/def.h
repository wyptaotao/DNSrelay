const int BUF_SIZE = 512;		//һ��DNS������󳤶�
const int MAX_ID_TABLE_SIZE=16; //ID��Ӧ���������
const int TTL = 5;				//���������Ϊ����
const int DNS_HEARDER_SIZE = 12; //DNSͷ�ֽ���

extern int debug_level;
extern SOCKET local_sock;
extern SOCKADDR clntAddr;
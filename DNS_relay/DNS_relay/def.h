const int BUF_SIZE = 512;		//һ��DNS������󳤶�
const int MAX_ID_TABLE_SIZE=16; //ID��Ӧ���������
const int TTL = 5;				//���������Ϊ����
const int DNS_HEARDER_SIZE = 12; //DNSͷ�ֽ���
const int DNS_PORT = 53;//DNS�˿�
const int MAX_CACHE_SIZE=5;//CACHE������
char DNS_Server_IP[16] = "10.3.9.4";//DNS������IP


extern int debug_level;
extern SOCKET local_sock;
extern SOCKADDR clntAddr;
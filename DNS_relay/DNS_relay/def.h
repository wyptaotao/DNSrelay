const int BUF_SIZE = 512;		//一个DNS包的最大长度
const int MAX_ID_TABLE_SIZE=16; //ID对应表最大容量
const int TTL = 5;				//生存期最大为五秒
const int DNS_HEARDER_SIZE = 12; //DNS头字节数
const int DNS_PORT = 53;//DNS端口
const int MAX_CACHE_SIZE=5;//CACHE表容量
char DNS_Server_IP[16] = "10.3.9.4";//DNS服务器IP


extern int debug_level;
extern SOCKET local_sock;
extern SOCKADDR clntAddr;
const int BUF_SIZE = 512;//一个DNS包的最大长度
const int MAX_ID_TABLE_SIZE=16;//ID对应表最大容量
const int TTL = 5;//生存期最大为五秒
int debug_level;
const int DNS_Header_Size = 12;//DNS头字节数
#include <stdio.h>
#include <winsock2.h>
#include "init.h"
#include "def.h"
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
char ip[2000][25];
char dn[2000][100];//domain name
SOCKET local_sock;
sockaddr_in localAddr;
SOCKADDR clntAddr;
int count = 0;			//记录本地记录条数
int debug_level;
char true_dns_server_ip[25];//真正的dns服务器的ip
int main()
{
	init_sock();
	init_data();
	while (1)
	{
		//query();

		//sendto(local_sock, buffer, strLen, 0, &clntAddr, nSize);
	}
	closesocket(local_sock);
	WSACleanup();
}
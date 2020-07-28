#include <stdio.h>
#include <winsock2.h>
#include "lib.h"
#include "def.h"
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
char ip[2000][20];
char dn[2000][100];//domain name
SOCKET local_sock;
sockaddr_in localAddr;
SOCKADDR clntAddr;
int count = 0;//记录本地记录条数

int main()
{
	init_sock();
	init_data();
	while (1)
	{
		//query();
		
		char buffer[BUF_SIZE];
		int nSize = sizeof(SOCKADDR);
		int strLen = recvfrom(local_sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
		printf("\n$$$$$$$$$$$$$$$  %s  $$$$$$$$$$$$$$\n", buffer);
		//sendto(local_sock, buffer, strLen, 0, &clntAddr, nSize);
	}
	closesocket(local_sock);
	WSACleanup();
}
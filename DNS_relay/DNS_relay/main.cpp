#include <stdio.h>
#include <winsock2.h>
#include "data.h"
#include "init.h"
#include "def.h"
#include"recv.h"
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
char ip[2000][25];
char dn[2000][100];//domain name
SOCKET local_sock;
sockaddr_in localAddr;
SOCKADDR clntAddr;
int count = 0;			//记录本地记录条数
int debug_level;
char true_dns_server_ip[25];//真正的dns服务器的ip
AllRecords r;
int main()
{
	init_sock();
	init_data();
	while (1)
	{
		//query();
		for (int i = 0; i < MAX_ID_TABLE_SIZE; i++)//initiate the ID_TRANS_TABLE
		{
			ID_Table[i].prev_ID = 0;
			ID_Table[i].status = TRUE;
			ID_Table[i].dead_time = 0;
			memset(&(ID_Table[i].client), 0, sizeof(SOCKADDR_IN));
		}
		printf("1\n");
		switch_pack();
		printf("5\n");
		char buffer[MAX_BUF_SIZE];
		int nSize = sizeof(SOCKADDR);
		int strLen = recvfrom(local_sock, buffer, MAX_BUF_SIZE, 0, &clntAddr, &nSize);
		printf("\n$$$$$$$$$$$$$$$  %s  $$$$$$$$$$$$$$\n", buffer);
		//sendto(local_sock, buffer, strLen, 0, &clntAddr, nSize);
	}
	closesocket(local_sock);
	WSACleanup();
}
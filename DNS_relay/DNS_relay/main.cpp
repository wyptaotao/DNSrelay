#include <stdio.h>
#include <winsock2.h>
#include "data.h"
#include "init.h"
#include "def.h"
#include"recv.h"
#include"print.h"
#include <iostream>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#pragma warning(disable:4996)
SOCKET local_sock;
sockaddr_in localAddr;
SOCKADDR clntAddr;
int count = 0;			//��¼���ؼ�¼����
int debug_level = 0;
char true_dns_server_ip[25];//������dns��������ip
AllRecords r;
char DNS_Server_IP[16] = "202.106.0.20";//DNS������IP
SOCKADDR_IN client_s;
sockaddr_in extern_id;
sockaddr_in external;
ID_Binding_Unit ID_Table[MAX_ID_TABLE_SIZE];//ID��Ӧ�󶨱�
SOCKET extern_sock;
Cache_Unit Cache[MAX_CACHE_SIZE];//�����
char file_open[30] = "dnsrelay.txt";

void Process_Parameters(int argc, char* argv[]) {
	int user_point = 0;
	if (argc == 1) {
		printf("Set DNS server : %s by default\n", DNS_Server_IP);
		return;
	}//û�в���
	if (argc > 1 && argv[1][0] == '-')
	{
		if (argv[1][1] == 'd') debug_level++; /* Debug level add to 1 */
		if (argv[1][2] == 'd') debug_level++; /* Debug level add to 2 */
	}//������-d


	if (argc == 3) {
		strcpy(DNS_Server_IP, argv[2]);
		printf("Set DNS server : %s\n", argv[2]);
	}

	if (argc == 4) {
		strcpy(DNS_Server_IP, argv[2]);
		strcpy(file_open, argv[3]);
		printf("Set DNS server : %s\n", argv[2]);
	}
	printf("debug_level:%d", debug_level);
}

int main(int argc, char* argv[])
{
	if (debug_level == 2)std::cout << "����������" << argc << std::endl;
	//std::cout << "argv[1][0]��" << argv[1][0] << "\nargv[1][1]��" << argv[1][1]<<std::endl;
	Process_Parameters(argc, argv);
	init_sock();
	init_data();
	while (1)
	{
		int a = query();
		if (debug_level == 2)std::cout << "��ѯ����Ϊ" << a << std::endl;
	}
	closesocket(local_sock);
	WSACleanup();
}
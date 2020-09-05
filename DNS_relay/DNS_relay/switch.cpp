#include <stdio.h>
#include <winsock2.h>
#include "lib.h"
#include "def.h"
#include "head.h"
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#pragma warning(disable:4996)
char ip[2000][20];
char dn[2000][100];//domain name
struct sockaddr_in localAddr;
int length_addr = sizeof(localAddr);
SOCKADDR clntAddr;
int count = 0;//记录本地记录条数

void color(short x)	//自定义函根据参数改变颜色 
{
	if (x >= 0 && x <= 15)//参数在0-15的范围颜色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);	//只有一个参数，改变字体颜色 
	else//默认的颜色白色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Set_ID_Expire(ID_Trans_Unit* record, int ttl)
{
	record->expire_time = time(NULL) + ttl;   /* expire_time = time now + time to live */
}

/* Check whether the record is expired */
int Get_ID_Expired(ID_Trans_Unit* record)
{
	return record->expire_time > 0 && time(NULL) > record->expire_time;
}

/* Register new ID into ID_Trans_Table */
unsigned short Register_New_ID(unsigned short ID, SOCKADDR_IN temp, BOOL if_done)
{
	int i = 0;
	for (i = 0; i != ID_TRANS_TABLE_SIZE; ++i)
	{
		/* Find out overdue record or a record which was analysed completely */
		if (Get_ID_Expired(&ID_Trans_Table[i]) == 1 || ID_Trans_Table[i].done == TRUE)
		{
			ID_Trans_Table[i].old_ID = ID;     /* Set ID */
			ID_Trans_Table[i].client = temp;   /* socket_addr */
			ID_Trans_Table[i].done = if_done;  /* Mark whether analysis completed */
			Set_ID_Expire(&ID_Trans_Table[i], ID_EXPIRE_TIME);
			ID_Count++;
			if (debug_level)
			{
				printf("New ID : %d recorded and registered successfully\n", i + 1);
				printf("ID number : %d\n", ID_Count);
			}
			break;
		}
	}
	if (i == ID_TRANS_TABLE_SIZE) /* Register failed */
		return 0;
	return (unsigned short)i + 1; /* Return new ID */
}

void Process_Parameters(int argc, char* argv[])
{
	int user_set_dns_flag = 0;
	if (argc > 1 && argv[1][0] == '-')
	{
		if (argv[1][1] == 'd') debug_level++; /* Debug level add to 1 */
		if (argv[1][2] == 'd') debug_level++; /* Debug level add to 2 */
		if (argc > 2)
		{
			user_set_dns_flag = 1; /* If user set the dns server ip address */
			strcpy(DNS_Server_IP, argv[2]);

		}
	}
	if (user_set_dns_flag) /* If user set the dns server ip address */
		printf("Set DNS server : %s\n", argv[2]);
	else /* If user do not set the dns server ip address, set it by default */
		printf("Set DNS server : %s by default\n", DNS_Server_IP);
	printf("Debug level : %d\n", debug_level);
}

void Convert_to_Url(char* buf, char* dest)
{
	int i = 0, j = 0, k = 0, len = strlen(buf);
	while (i < len)
	{
		if (buf[i] > 0 && buf[i] <= 63) /* Count */
		{
			for (j = buf[i], i++; j > 0; j--, i++, k++) /* Copy the url */
				dest[k] = buf[i];
		}
		if (buf[i] != 0) /* If this is not the end, put a dot into dest */
		{
			dest[k] = '.';
			k++;
		}
	}
	dest[k] = '\0'; /* Set the end */
}

void switch_pack() {
	char buf[BUF_SIZE];
	char url[65];
	memset(buf, 0, sizeof(buf));
	int packlen = 0;
	debug_level = 1;
	packlen = recvfrom(local_sock, buf, sizeof buf, 0, (struct sockaddr*)&localAddr, &length_addr);/* Receive packet from client */
	char ori_url[165];
	if (packlen > 0) {
		memcpy(ori_url, &(buf[DNS_HEAD_SIZE]), packlen); /* Get original url from buf */
		Convert_to_Url(ori_url, url); /* Convert original url to url */
		if (debug_level)
		{
			printf("\n\nReceive from client [IP:%s]\n", inet_ntoa(client.sin_addr));

			/* Output time now */
			time_t t = time(NULL);
			char temp[64];
			strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
			printf("|%s|\n", temp);

			printf("Receive from client [Query :  ");
			color(4);
			printf(" %s", url);
			color(16);
			printf("]\n");
		}

		printf("[Url : %s] not in local data and cache\n", url);
		unsigned short* pID = (unsigned short*)malloc(sizeof(unsigned short));
		memcpy(pID, buf, sizeof(unsigned short)); /* Record ID */
		unsigned short nID = Register_New_ID(*pID, client, FALSE); /* Register in the ID transfer table */
		if (nID == 0)
		{
			if (debug_level >= 1)
				printf("ID Register failed. The ID transfer table is already full.\n");
		}
		else
		{
			memcpy(buf, &nID, sizeof(unsigned short));
			packlen = sendto(extern_sock, buf, packlen, 0, (struct sockaddr*)&extern_id, sizeof(extern_id));/* Send the request to external DNS server */
			if (debug_level >= 1)
				printf("Send to external DNS server [Url : ");
				color(2);	
				printf(" %s", url);
				color(16);
				printf("]\n");
		}
		free(pID);
	}
}

int main()
{
	init_sock();
	init_data();
	while (1)
	{
		//query();
		for (int i = 0; i < ID_TRANS_TABLE_SIZE; i++)//initiate the ID_TRANS_TABLE
		{
			ID_Trans_Table[i].old_ID = 0;
			ID_Trans_Table[i].done = TRUE;
			ID_Trans_Table[i].expire_time = 0;
			memset(&(ID_Trans_Table[i].client), 0, sizeof(SOCKADDR_IN));
		}

		extern_id.sin_family = AF_INET;                         // Set the family as TCP/IP 
		extern_id.sin_addr.s_addr = inet_addr(DNS_Server_IP);   // Set to the IP of extern DNS server 
		extern_id.sin_port = htons(DNS_PORT);                   // Set the port as DNS port (53) 
		printf("1\n");
		switch_pack();
		printf("5\n");
		char buffer[BUF_SIZE];
		int nSize = sizeof(SOCKADDR);
		int strLen = recvfrom(local_sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
		printf("\n$$$$$$$$$$$$$$$  %s  $$$$$$$$$$$$$$\n", buffer);
		//sendto(local_sock, buffer, strLen, 0, &clntAddr, nSize);
	}
	closesocket(local_sock);
	WSACleanup();
}
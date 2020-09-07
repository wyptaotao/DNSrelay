#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <WS2tcpip.h>
#include <time.h>
#include "def.h"
#include "data.h"
#include "print.h"
#include "recv.h"
#pragma warning(disable:4996)

extern char DNS_Server_IP[16];
extern SOCKET local_sock;
extern SOCKADDR clntAddr;
extern int debug_level;
extern sockaddr_in localAddr;
extern int count;
extern AllRecords r;
int length_add = sizeof(localAddr);
extern sockaddr_in extern_id;
extern sockaddr_in external;
int length_c = sizeof(external);
extern SOCKET extern_sock;

char buffer[MAX_BUF_SIZE] = { 0 };
int nSize = sizeof(SOCKADDR);
int length;
char sendbuf[MAX_BUF_SIZE];


void Output_Packet(char* buf, int length)
{
	unsigned char unit;
	printf("Packet length = %d\n", length);
	printf("Details of the package:\n");
	for (int i = 0; i < length; i++)
	{
		unit = (unsigned char)buf[i];
		printf("%02x ", unit);
	}
	printf("\n");
}

int send_to_client(Record record) {
	char sendbuf[MAX_BUF_SIZE];
	memcpy(sendbuf, buffer, length); /* Copy the request packet */
	unsigned short a = htons(0x8180);
	memcpy(&sendbuf[2], &a, sizeof(unsigned short)); /* Set the flags of Head */
	if (record.ip == "0.0.0.0")    /* Judge if the Url should be shielded */
		a = htons(0x0000);	/* Shielding function : set the number of answer to 0 */
	else a = htons(0x0001);	/* Server function : set the number of answer to 1 */
	memcpy(&sendbuf[6], &a, sizeof(unsigned short));

	int curLen = 0;
	char answer[16];
	unsigned short Name = htons(0xc00c);  /* Pointer of domain */
	memcpy(answer, &Name, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short TypeA = htons(0x0001);  /* Type */
	memcpy(answer + curLen, &TypeA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short ClassA = htons(0x0001);  /* Class */
	memcpy(answer + curLen, &ClassA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned long timeLive = htonl(0x7b); /* Time to live */
	memcpy(answer + curLen, &timeLive, sizeof(unsigned long));
	curLen += sizeof(unsigned long);

	unsigned short IPLen = htons(0x0004);  /* Data length */
	memcpy(answer + curLen, &IPLen, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned long IP;
	inet_pton(AF_INET, record.ip, &IP); /* Actually data is IP */
	//转换由strptr指针所指的字符串，并通过addrptr指针存放二进制结果。
	//若成功则返回1, 否则如果对所指定的family而言输入的字符串不是有效的表达式，那么值为0。
	memcpy(answer + curLen, &IP, sizeof(unsigned long));
	curLen += sizeof(unsigned long);
	curLen += length;
	memcpy(sendbuf + length, answer, sizeof(answer));

	return length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */
}


int query()
{
	length = recvfrom(local_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&localAddr, &nSize);
	char url[512] = { 0 };
	char ori_url[512] = { 0 };		 /* Original url */
	memcpy(ori_url, &(buffer[DNS_HEADER_SIZE]), length); /* Get original url from packet */
	Transfer_URL(ori_url, url);		 /* Convert original url to normal url */
	if (debug_level)
	{
		//将IP地址从二进制转成表达格式
		char str[20] = { 0 };
		printf("\n\n---- Recv : Client [IP:%s]----\n", inet_ntop(AF_INET, &localAddr.sin_addr, str, INET_ADDRSTRLEN));

		/* Output time now */
		print_time();

		printf("Receive from client [Query : %s]\n", url);
	}

	//先在cache里找
	for (int i = 0; i < MAX_CACHE_SIZE; i++) {
		if (strcmp(url, Cache[i].inf.dn) == 0)/*匹配cache里的域名*/ {
			send_to_client(Cache[i].inf);
			return 1;
		}
	}
	/*还没写*/

	for (int i = 0; i < r.count; i++)
	{
		if (strcmp(url, r.record[i].dn) == 0)//找到,回送给本地
		{
			send_to_client(r.record[i]);
			return 2;
		}


	}



	//在DNS服务器中寻找
	if (length > 0) {
		char buf[MAX_BUF_SIZE];
		memset(buf, 0, sizeof(buf));
		memcpy(buf, buffer, MAX_BUF_SIZE);
		debug_level = 1;
		if (debug_level)
		{
			printf("\n\nReceive from client [IP:%s]\n", inet_ntoa(localAddr.sin_addr));

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
		memcpy(pID, buf, sizeof(unsigned short));//记录下当前包id
		unsigned short nID = Bind_ID(*pID, client_s);//绑定转发查询
		if (nID == 404)
		{
			if (debug_level >= 1)
				printf("ID Register failed. The ID transfer table is already full.\n");
		}
		else
		{
			//发送给外部
			memcpy(buf, &nID, sizeof(unsigned short));
			int length_s = sendto(extern_sock, buf, length, 0, (struct sockaddr*)&extern_id, sizeof(extern_id));//发送给DNS服务器
			if (debug_level >= 1) {
				printf("Send to external DNS server [Url : ");
				color(2);
				printf(" %s", url);
				color(16);
				printf("]\n");
				printf("ID numbrer:%d\n", nID);
			}
		}
		free(pID);
	}//czk代码结束
	//从外部接收

	char recvBuf[MAX_BUF_SIZE];
	memset(recvBuf, 0, MAX_BUF_SIZE);
	int recvlen = -1;
	recvlen = recvfrom(extern_sock, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&external, &length_c); /* Receive DNS packet from exterior */
	if (recvlen > -1)
	{
		if (debug_level)
		{
			printf("\n\n----从外部DNS接收的[IP:%s]----\n", inet_ntoa(external.sin_addr));

			/* Output time now */
			time_t t = time(NULL);
			char temp[64];
			strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
			printf("%s\n", temp);

			if (debug_level == 2)
				Output_Packet(recvBuf, recvlen);
		}
		unsigned short* pID = (unsigned short*)malloc(sizeof(unsigned short));
		memcpy(pID, recvBuf, sizeof(unsigned short));
		int id_index = (*pID) - 1;
		free(pID);

		memcpy(recvBuf, &ID_Table[id_index].prev_ID, sizeof(unsigned short));
		if (debug_level >= 1) { ; }
		//printf("#ID Count : %d\n", ID_Count);
		ID_Table[id_index].status = TRUE;

		//客户端信息
		sockaddr_in client_back = ID_Table[id_index].client;

		//问题和回答的数量
		int nquery = ntohs(*((unsigned short*)(recvBuf + 4))), nresponse = ntohs(*((unsigned short*)(recvBuf + 6)));
		char* p = recvBuf + 12; /* p point to the Quetion field */
		char ip[16];
		int ip1, ip2, ip3, ip4;

		char new_url[200];
		/* Read urls from queries, but only record last url */
		for (int i = 0; i < nquery; i++)
		{
			Transfer_URL(p, new_url);
			while (*p > 0)
				p += (*p) + 1;
			p += 5; /* Point to the next query */
		}

		if (nresponse > 0 && debug_level >= 1)
			printf("外部DNS服务器返回信息 [Url : %s]\n", new_url);

		//分析包
		for (int i = 0; i < nresponse; ++i)
		{
			if ((unsigned char)*p == 0xc0) /* The name field is pointer */
				p += 2;
			else /* The name field is Url */
			{
				while (*p > 0)
					p += (*p) + 1;
				++p;
			}
			unsigned short resp_type = ntohs(*(unsigned short*)p);  /* Type */
			p += 2;
			unsigned short resp_class = ntohs(*(unsigned short*)p); /* Class */
			p += 2;
			unsigned short high = ntohs(*(unsigned short*)p); /* TTL high bit */
			p += 2;
			unsigned short low = ntohs(*(unsigned short*)p);  /* TTL low bit */
			p += 2;
			int ttl = (((int)high) << 16) | low;    /* TTL combinate */
			int datalen = ntohs(*(unsigned short*)p);  /* Data length */
			p += 2;
			if (debug_level >= 2)
				printf("Type -> %d,  Class -> %d,  TTL -> %d\n", resp_type, resp_class, ttl);

			if (resp_type == 1) /* Type A, the response is IPv4 address */
			{
				ip1 = (unsigned char)*p++;
				ip2 = (unsigned char)*p++;
				ip3 = (unsigned char)*p++;
				ip4 = (unsigned char)*p++;

				sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
				if (debug_level)
					printf("IP address : %d.%d.%d.%d\n", ip1, ip2, ip3, ip4);

				/* Add record to cache */
				//在这里写向cache中加入数据
				break;
			}
			else p += datalen;  /* If type is not A, then ignore it */
		}

		/* Send packet to client */
		length = sendto(local_sock, recvBuf, length, 0, (SOCKADDR*)&localAddr, sizeof(localAddr));
	}
	else {
		printf("未收到！！！\n");
	}


	return 3;
}





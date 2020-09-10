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

void print_type(int type) {
	switch (type) {
	case 1: {
		printf("ip类型为IPV4\n");
		break;
	}
	case 2:	{
		printf("ip类型为NS\n");
		break;
	}
	case 5: {
		printf("ip类型为CNAME\n");
		break;
	}
	case 15: {
		printf("ip类型为MX\n");
		break;
	}
	case 28: {
		printf("ip类型为IPV6\n");
		break;
	}
	}
}
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
unsigned short d_to_h(int dec) {
	return (unsigned short)dec;
}
int send_cache_to_client(Cache_Unit cache_unit){
	char sendbuf[MAX_BUF_SIZE];
	memcpy(sendbuf, buffer, length); /* Copy the request packet */
	unsigned short a = htons(0x8180);
	memcpy(&sendbuf[2], &a, sizeof(unsigned short)); /* Set the flags of Head */
	if (strcmp(cache_unit.ip[0] ,"0.0.0.0") == 0)    /* Judge if the Url should be shielded */
		a = htons(0x0000);	/* Shielding function : set the number of answer to 0 */
	else 
		a = htons(0x0000|d_to_h(cache_unit.ip_count)); 
		memcpy(&sendbuf[6], &a, sizeof(unsigned short));
		int curLen = 0;
		char answer[100]; memset(answer, 0, sizeof(answer));
		for (int i = 0; i < cache_unit.ip_count; i++) {
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
			inet_pton(AF_INET, cache_unit.ip[i], &IP); /* Actually data is IP */
			//转换由strptr指针所指的字符串，并通过addrptr指针存放二进制结果。
			//若成功则返回1, 否则如果对所指定的family而言输入的字符串不是有效的表达式，那么值为0。
			memcpy(answer + curLen, &IP, sizeof(unsigned long));
			curLen += sizeof(unsigned long);
		}
		curLen += length;
		memcpy(sendbuf + length, answer, sizeof(answer));


		return length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */
	
}
int send_to_client(Record record) {

	std::cout << "找到啦！" <<std:: endl;
	char sendbuf[MAX_BUF_SIZE];
	memcpy(sendbuf, buffer, length); /* Copy the request packet */


	unsigned short Flags;//在本地找到，为权威答案 //unsigned short Flags = htons(0x8180);	//response

	unsigned short ancount;
	if (strcmp(record.ip, "0.0.0.0") == 0)/* Judge if the Url should be shielded */
	{
		Flags = htons(0x8583);
		std::cout << "该域名为无效域名。" << std::endl;
		memcpy(&sendbuf[2], &Flags, sizeof(unsigned short)); /* Set the flags of Head */

		ancount = htons(0x0000);			/* Shielding function : set the number of answer to 0 */
		memcpy(&sendbuf[6], &ancount, sizeof(unsigned short));
	}
	else
	{
		Flags = htons(0x8580);
		memcpy(&sendbuf[2], &Flags, sizeof(unsigned short)); /* Set the flags of Head */

		ancount = htons(0x0001);			/* Server function : set the number of answer to 1 */
		memcpy(&sendbuf[6], &ancount, sizeof(unsigned short));
	}



	//构建answer字段
	int curLen = 0;
	char answer[16] = { 0 };
	unsigned short Name = htons(0xc00c);  /* Pointer of domain */	///???
	memcpy(answer, &Name, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short TypeA = htons(0x0001);  /* Type设为1表示A类型 */
	memcpy(answer + curLen, &TypeA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short ClassA = htons(0x0001);  /* Class设为1表示IN 因特网 */
	memcpy(answer + curLen, &ClassA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned long TTL = htonl(0x7b); /* Time to live */
	//unsigned long TTL = htonl(0x0005);
	memcpy(answer + curLen, &TTL, sizeof(unsigned long));//为什么是unsigned long
	curLen += sizeof(unsigned long);

	//unsigned short TTL = htons(0x0005);
	//memcpy(answer + curLen, &TTL, sizeof(unsigned short));
	//curLen += sizeof(unsigned short);

	unsigned short IPLen = htons(0x0004);  /* Data length ,对于A类型，值为IP地址*/
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

	length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */

	return length;
}


int query()
{

	timeval tv = { 1 ,0 };
	setsockopt(local_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(timeval));

	length = recvfrom(local_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&localAddr, &nSize);
	if (length > 0) {
		color(9);
		if(debug_level==2)printf("当前时间%d\n", time(NULL));
		char url[512] = { 0 };
		char ori_url[512] = { 0 };		 /* Original url */
		memcpy(ori_url, &(buffer[DNS_HEADER_SIZE]), length); /* Get original url from packet */
		Transfer_URL(ori_url, url);		 /* Convert original url to normal url */
		if (debug_level)
		{
			//将IP地址从二进制转成表达格式
			char str[20] = { 0 };
			printf("\n\n----从客户端接收 [IP:%s]----\n", inet_ntop(AF_INET, &localAddr.sin_addr, str, INET_ADDRSTRLEN));

			/* Output time now */

			printf("从客户端接收 [Query : %s]\n", url);
		}

		//先在cache里找
		for (int i = 0; i < MAX_CACHE_SIZE; i++) {
			if(debug_level==2)std::cout << "当前cache域名：" << Cache[i].dn << std::endl;
			if (strcmp(url, Cache[i].dn)==0)/*匹配cache里的域名*/ {
				Cache[i].ttl = CACHE_TTL + 1;
				LFU_Refresh();
				send_cache_to_client(Cache[i]);
				if(debug_level)printf("在cache中成功找到，发给本地！\n");
				return 1;
			}
		}
		if (debug_level == 2)printf("找完缓存，当前时间%d\n", time(NULL));

		for (int i = 0; i < r.count; i++)
		{
			if (strcmp(url, r.record[i].dn) == 0)//找到,回送给本地
			{
				if (debug_level == 2)printf("发送给本地前，当前时间%d\n", time(NULL));

				send_to_client(r.record[i]);
				if (debug_level == 2)printf("发送给本地后，当前时间%d\n", time(NULL));
				if (debug_level)printf("在本地文件中成功找到，发给本地！\n");

				return 2;
			}


		}



	//在DNS服务器中寻找
		if (debug_level == 2)printf("开始在dns服务器查询，当前时间%d\n", time(NULL));

		char buf[MAX_BUF_SIZE];
		memset(buf, 0, sizeof(buf));
		memcpy(buf, buffer, MAX_BUF_SIZE);
		if (debug_level)
		{
			time_t t = time(NULL);
			char temp[64];
			strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
			printf("|%s|\n", temp);

			printf("从客户端收到包 [Query :  ");
			color(12);
			printf(" %s", url);
			color(9);
			printf("]\n");
		}

		printf("[Url : %s] cache和本地文件中都没有\n", url);
		unsigned short* pID = (unsigned short*)malloc(sizeof(unsigned short));
		memcpy(pID, buf, sizeof(unsigned short));//记录下当前包id
		unsigned short nID = Bind_ID(*pID, client_s);//绑定转发查询
		if (debug_level == 2)printf("PID为：%d", *pID);
		if (nID == 404)
		{
			if (debug_level >= 1)
				printf("ID Register failed. The ID transfer table is already full.\n");
		}
		else
		{
		//发送给外部
			//if (debug_level == 2)printf("准备发送给外部，当前时间%d\n", time(NULL));

			memcpy(buf, &nID, sizeof(unsigned short));
			int length_s = -1;
			length_s = sendto(extern_sock, buf, length, 0, (struct sockaddr*)&extern_id, sizeof(extern_id));//发送给DNS服务器
			if (debug_level == 2)
			//printf("给外部发送完，当前时间%d\n", time(NULL));

			if (length_s != -1)printf("成功发送给DNS服务器\n");
			if (debug_level >= 1) {
				printf("Send to external DNS server [Url : ");
				color(2);
				printf(" %s", url);
				color(9);
				printf("]\n");
				printf("ID numbrer:%d\n", nID);
			}
		}
		free(pID);
		color(16);
	}//czk代码结束
	//从外部接收

	char recvBuf[MAX_BUF_SIZE];
	memset(recvBuf, 0, MAX_BUF_SIZE);
	int recvlen = -1;
	//if (debug_level == 2)printf("准备从外部接受：当前时间%d\n", time(NULL));

	setsockopt(extern_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));

	recvlen = recvfrom(extern_sock, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&external, &length_c); /* Receive DNS packet from exterior */
	Record now;
	strcpy(now.ip, inet_ntoa(external.sin_addr));
	//if (debug_level == 2)printf("从外部接受完：当前时间%d\n", time(NULL));
	if (recvlen > -1)
	{
		if (debug_level)
		{
			color(6);
			printf("\n\n----从外部DNS接收的[IP:%s]----\n", inet_ntoa(external.sin_addr));

			/* Output time now */
			time_t t = time(NULL);
			char temp[64];
			strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
			printf("%s\n", temp);

			if (debug_level == 2) { Output_Packet(recvBuf, recvlen); }
		}
		unsigned short* pID = (unsigned short*)malloc(sizeof(unsigned short));
		memcpy(pID, recvBuf, sizeof(unsigned short));
		int id_index = (*pID) ;
		free(pID);
		memcpy(recvBuf, &ID_Table[id_index].prev_ID, sizeof(unsigned short));
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

			if(debug_level)print_type(resp_type);
			if (resp_type == 1) /* Type A, the response is IPv4 address */
			{
				ip1 = (unsigned char)*p++;
				ip2 = (unsigned char)*p++;
				ip3 = (unsigned char)*p++;
				ip4 = (unsigned char)*p++;

				sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
				if (debug_level) {
					printf("IP address : %d.%d.%d.%d\n", ip1, ip2, ip3, ip4);
				}
				if (r.count == MAX_RECORDS_NUM) {
					Add_To_Txt(ip, new_url);
					Add_To_Table(&r, ip, new_url);
				}//向txt文件和本地查询表中加入新查询结果，若本地表满则不再添加
				Record aaa;
				strcpy(aaa.dn, new_url);
				strcpy(aaa.ip, ip);

				Add_To_Cache(aaa);
				//在这里cache中加入数据
				break;
			}
			else p += datalen;  /* If type is not A, then ignore it */
		}

		/* Send packet to client */
		int lensss=-1;
		lensss = sendto(local_sock, recvBuf, recvlen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr));
		if(debug_level){if (lensss != -1)printf("成功发送！长度为%d\n", recvlen); }
		
		color(16);
	}
	else {
		color(4);
		if(debug_level) printf("\n未收到外部包！！！\nrecvlen=%d\n",recvlen);
		color(16);
	}
	

	return 3;
}





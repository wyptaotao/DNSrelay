#include<iostream>
#include <winsock2.h>
#include <string.h>
#include <WS2tcpip.h>
#include<time.h>
#include "def.h"
#include "data.h"
#include "print.h"

extern char DNS_Server_IP[16];
extern SOCKET local_sock;
extern SOCKADDR clntAddr;
extern int debug_level;
extern sockaddr_in localAddr;
extern int count;
extern AllRecords r;

int query()
{
	char buffer[MAX_BUF_SIZE] = { 0 };
	int nSize = sizeof(SOCKADDR);
	int length = recvfrom(local_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&localAddr, &nSize);

	//printf("the strLen is : %d\n", length);
	//for (int i = 0; i < length; i++)
	//{
	//	printf("@@@@@@@@@ %c @@@@@@@@@@@@@\n", buffer[i]);//加点注释
	//}
	//printf("\n@@@@     end      @@@@\n");

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
	/*还没写*/

	for (int i = 0; i < r.count; i++)
	{
		if (strcmp(url, r.record[i].dn) == 0)//找到,回送给本地
		{

			char sendbuf[MAX_BUF_SIZE];
			memcpy(sendbuf, buffer, length); /* Copy the request packet */
			unsigned short a = htons(0x8180);
			memcpy(&sendbuf[2], &a, sizeof(unsigned short)); /* Set the flags of Head */


		//if (strcmp(r.record[i].ip, "0.0.0.0") == 0)//为无效域名
		//{
		//	

			if (r.record[i].ip == "0.0.0.0")    /* Judge if the Url should be shielded */
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
			inet_pton(AF_INET, r.record[i].ip, &IP); /* Actually data is IP */
			//转换由strptr指针所指的字符串，并通过addrptr指针存放二进制结果。
			//若成功则返回1, 否则如果对所指定的family而言输入的字符串不是有效的表达式，那么值为0。
			memcpy(answer + curLen, &IP, sizeof(unsigned long));
			curLen += sizeof(unsigned long);
			curLen += length;
			memcpy(sendbuf + length, answer, sizeof(answer));

			length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */
		}


	}

	return 0;
}




//if (/*ret为从客户端收到包*/)//为查询请求
//{
//	//先从本地程序里开始查
//	if (/*从本地程序里查到*/)
//	{
//		//记得判断查到的IP地址是不是0.0.0.0
//		//sento();回发给客户端
//	}
//	else///*从本地程序里没查到*/
//	{
//		//绑定ID绑定单元
//		//根据绑定后的返回的ID，修改包的内容（修改id等啥的）
//		//发送给实际的DNS服务器
//	}
//}

//if (/*ret为从实际的服务器端收到的包*/)//为中继的回答
//{

//	//判断收到包里的域名和要查询的是否相同，不同则丢弃包，相同则继续
//	//一些操作：找到旧ID啥的构造好回送包
//	//回送给客户端
//}
#include<iostream>
#include "def.h"
#include <winsock2.h>

extern SOCKET local_sock;
extern SOCKADDR clntAddr;
void query()
{
	char buffer[MAX_BUF_SIZE] = { 0 };
	int nSize = sizeof(SOCKADDR);
	int strLen = recvfrom(local_sock, buffer, sizeof(buffer), 0, &clntAddr, &nSize);

	printf("the strLen is : %d\n", strLen);
	for (int i = 0; i < strLen; i++)
	{
		printf("@@@@@@@@@ %c @@@@@@@@@@@@@\n", buffer[i]);//加点注释
	}
	printf("\n@@@@     end      @@@@\n");

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
}

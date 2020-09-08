#include <stdio.h>
#include <winsock2.h>
#include <time.h> 
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#include <time.h>
#include "def.h"
#include "data.h"
#pragma warning(disable:4996)
extern char ip[2000][20];
extern char dn[2000][100];//domain name
extern struct sockaddr_in localAddr;
int length_addr = sizeof(localAddr);
extern SOCKADDR clntAddr;
extern int count;//��¼���ؼ�¼����
extern char DNS_Server_IP[16];//DNS������IP
extern SOCKET extern_sock;

void color(short x)	//�Զ��庯���ݲ����ı���ɫ 
{
	if (x >= 0 && x <= 15)//������0-15�ķ�Χ��ɫ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);	//ֻ��һ���������ı�������ɫ 
	else//Ĭ�ϵ���ɫ��ɫ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}



void switch_pack() {
	char buf[MAX_BUF_SIZE];
	char url[65];
	memset(buf, 0, sizeof(buf));
	int packlen = 0;
	debug_level = 1;
	packlen = recvfrom(local_sock, buf, sizeof buf, 0, (struct sockaddr*)&localAddr, &length_addr);/* Receive packet from client */
	char ori_url[165];
	if (packlen > 0) {
		memcpy(ori_url, &(buf[DNS_HEADER_SIZE]), packlen);//��ȡurl
		Transfer_URL(ori_url, url);//ת��Ϊ����url�ַ���
		if (debug_level)
		{
			printf("\n\nReceive from client [IP:%s]\n", inet_ntoa(client_s.sin_addr));

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
		memcpy(pID, buf, sizeof(unsigned short));//��¼�µ�ǰ��id
		unsigned short nID = Bind_ID(*pID, client_s);//��ת����ѯ
		if (nID == 404)
		{
			if (debug_level >= 1)
				printf("ID Register failed. The ID transfer table is already full.\n");
		}
		else
		{
			memcpy(buf, &nID, sizeof(unsigned short));
			packlen = sendto(extern_sock, buf, packlen, 0, (struct sockaddr*)&extern_id, sizeof(extern_id));//���͸�DNS������
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

	}
}
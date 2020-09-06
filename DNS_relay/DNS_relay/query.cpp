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

char buffer[MAX_BUF_SIZE] = { 0 };
int nSize = sizeof(SOCKADDR);
int length;
char sendbuf[MAX_BUF_SIZE];
int query()
{
	
	//printf("the strLen is : %d\n", length);
	//for (int i = 0; i < length; i++)
	//{
	//	printf("@@@@@@@@@ %c @@@@@@@@@@@@@\n", buffer[i]);//�ӵ�ע��
	//}
	//printf("\n@@@@     end      @@@@\n");
	length = recvfrom(local_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&localAddr, &nSize);
	char url[512] = { 0 };
	char ori_url[512] = { 0 };		 /* Original url */
	memcpy(ori_url, &(buffer[DNS_HEARDER_SIZE]), length); /* Get original url from packet */
	Transfer_URL(ori_url, url);		 /* Convert original url to normal url */
	if (debug_level)
	{
		//��IP��ַ�Ӷ�����ת�ɱ���ʽ
		char str[20] = { 0 };
		printf("\n\n---- Recv : Client [IP:%s]----\n", inet_ntop(AF_INET, &localAddr.sin_addr, str, INET_ADDRSTRLEN));

		/* Output time now */
		print_time();

		printf("Receive from client [Query : %s]\n", url);
	}

	//����cache����
	for (int i = 0; i < MAX_CACHE_SIZE; i++) {
		if (strcmp(url, Cache[i].inf.dn))/*ƥ��cache�������*/ {
			send_to_client(Cache[i].inf);
			Cache[i].ttl = TTL + 1;
			LFU_Refresh();//ˢ��cache
		}
	}

	for (int i = 0; i < r.count; i++)
	{
		if (strcmp(url, r.record[i].dn) == 0)//�ҵ�,���͸�����
		{
			send_to_client(r.record[i]);
			Add_To_Cache(r.record[i]);//���뻺��
		}
	}

	return 0;
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
	//ת����strptrָ����ָ���ַ�������ͨ��addrptrָ���Ŷ����ƽ����
	//���ɹ��򷵻�1, �����������ָ����family����������ַ���������Ч�ı��ʽ����ôֵΪ0��
	memcpy(answer + curLen, &IP, sizeof(unsigned long));
	curLen += sizeof(unsigned long);
	curLen += length;
	memcpy(sendbuf + length, answer, sizeof(answer));

	return length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */
}


//if (/*retΪ�ӿͻ����յ���*/)//Ϊ��ѯ����
//{
//	//�ȴӱ��س����￪ʼ��
//	if (/*�ӱ��س�����鵽*/)
//	{
//		//�ǵ��жϲ鵽��IP��ַ�ǲ���0.0.0.0
//		//sento();�ط����ͻ���
//	}
//	else///*�ӱ��س�����û�鵽*/
//	{
//		//��ID�󶨵�Ԫ
//		//���ݰ󶨺�ķ��ص�ID���޸İ������ݣ��޸�id��ɶ�ģ�
//		//���͸�ʵ�ʵ�DNS������
//	}
//}

//if (/*retΪ��ʵ�ʵķ��������յ��İ�*/)//Ϊ�м̵Ļش�
//{

//	//�ж��յ������������Ҫ��ѯ���Ƿ���ͬ����ͬ����������ͬ�����
//	//һЩ�������ҵ���IDɶ�Ĺ���û��Ͱ�
//	//���͸��ͻ���
//}
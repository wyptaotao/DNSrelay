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
		printf("ip����ΪIPV4\n");
		break;
	}
	case 2:	{
		printf("ip����ΪNS\n");
		break;
	}
	case 5: {
		printf("ip����ΪCNAME\n");
		break;
	}
	case 15: {
		printf("ip����ΪMX\n");
		break;
	}
	case 28: {
		printf("ip����ΪIPV6\n");
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
			//ת����strptrָ����ָ���ַ�������ͨ��addrptrָ���Ŷ����ƽ����
			//���ɹ��򷵻�1, �����������ָ����family����������ַ���������Ч�ı��ʽ����ôֵΪ0��
			memcpy(answer + curLen, &IP, sizeof(unsigned long));
			curLen += sizeof(unsigned long);
		}
		curLen += length;
		memcpy(sendbuf + length, answer, sizeof(answer));


		return length = sendto(local_sock, sendbuf, curLen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr)); /* Send the packet to client */
	
}
int send_to_client(Record record) {

	std::cout << "�ҵ�����" <<std:: endl;
	char sendbuf[MAX_BUF_SIZE];
	memcpy(sendbuf, buffer, length); /* Copy the request packet */


	unsigned short Flags;//�ڱ����ҵ���ΪȨ���� //unsigned short Flags = htons(0x8180);	//response

	unsigned short ancount;
	if (strcmp(record.ip, "0.0.0.0") == 0)/* Judge if the Url should be shielded */
	{
		Flags = htons(0x8583);
		std::cout << "������Ϊ��Ч������" << std::endl;
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



	//����answer�ֶ�
	int curLen = 0;
	char answer[16] = { 0 };
	unsigned short Name = htons(0xc00c);  /* Pointer of domain */	///???
	memcpy(answer, &Name, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short TypeA = htons(0x0001);  /* Type��Ϊ1��ʾA���� */
	memcpy(answer + curLen, &TypeA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned short ClassA = htons(0x0001);  /* Class��Ϊ1��ʾIN ������ */
	memcpy(answer + curLen, &ClassA, sizeof(unsigned short));
	curLen += sizeof(unsigned short);

	unsigned long TTL = htonl(0x7b); /* Time to live */
	//unsigned long TTL = htonl(0x0005);
	memcpy(answer + curLen, &TTL, sizeof(unsigned long));//Ϊʲô��unsigned long
	curLen += sizeof(unsigned long);

	//unsigned short TTL = htons(0x0005);
	//memcpy(answer + curLen, &TTL, sizeof(unsigned short));
	//curLen += sizeof(unsigned short);

	unsigned short IPLen = htons(0x0004);  /* Data length ,����A���ͣ�ֵΪIP��ַ*/
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
		if(debug_level==2)printf("��ǰʱ��%d\n", time(NULL));
		char url[512] = { 0 };
		char ori_url[512] = { 0 };		 /* Original url */
		memcpy(ori_url, &(buffer[DNS_HEADER_SIZE]), length); /* Get original url from packet */
		Transfer_URL(ori_url, url);		 /* Convert original url to normal url */
		if (debug_level)
		{
			//��IP��ַ�Ӷ�����ת�ɱ���ʽ
			char str[20] = { 0 };
			printf("\n\n----�ӿͻ��˽��� [IP:%s]----\n", inet_ntop(AF_INET, &localAddr.sin_addr, str, INET_ADDRSTRLEN));

			/* Output time now */

			printf("�ӿͻ��˽��� [Query : %s]\n", url);
		}

		//����cache����
		for (int i = 0; i < MAX_CACHE_SIZE; i++) {
			if(debug_level==2)std::cout << "��ǰcache������" << Cache[i].dn << std::endl;
			if (strcmp(url, Cache[i].dn)==0)/*ƥ��cache�������*/ {
				Cache[i].ttl = CACHE_TTL + 1;
				LFU_Refresh();
				send_cache_to_client(Cache[i]);
				if(debug_level)printf("��cache�гɹ��ҵ����������أ�\n");
				return 1;
			}
		}
		if (debug_level == 2)printf("���껺�棬��ǰʱ��%d\n", time(NULL));

		for (int i = 0; i < r.count; i++)
		{
			if (strcmp(url, r.record[i].dn) == 0)//�ҵ�,���͸�����
			{
				if (debug_level == 2)printf("���͸�����ǰ����ǰʱ��%d\n", time(NULL));

				send_to_client(r.record[i]);
				if (debug_level == 2)printf("���͸����غ󣬵�ǰʱ��%d\n", time(NULL));
				if (debug_level)printf("�ڱ����ļ��гɹ��ҵ����������أ�\n");

				return 2;
			}


		}



	//��DNS��������Ѱ��
		if (debug_level == 2)printf("��ʼ��dns��������ѯ����ǰʱ��%d\n", time(NULL));

		char buf[MAX_BUF_SIZE];
		memset(buf, 0, sizeof(buf));
		memcpy(buf, buffer, MAX_BUF_SIZE);
		if (debug_level)
		{
			time_t t = time(NULL);
			char temp[64];
			strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
			printf("|%s|\n", temp);

			printf("�ӿͻ����յ��� [Query :  ");
			color(12);
			printf(" %s", url);
			color(9);
			printf("]\n");
		}

		printf("[Url : %s] cache�ͱ����ļ��ж�û��\n", url);
		unsigned short* pID = (unsigned short*)malloc(sizeof(unsigned short));
		memcpy(pID, buf, sizeof(unsigned short));//��¼�µ�ǰ��id
		unsigned short nID = Bind_ID(*pID, client_s);//��ת����ѯ
		if (debug_level == 2)printf("PIDΪ��%d", *pID);
		if (nID == 404)
		{
			if (debug_level >= 1)
				printf("ID Register failed. The ID transfer table is already full.\n");
		}
		else
		{
		//���͸��ⲿ
			//if (debug_level == 2)printf("׼�����͸��ⲿ����ǰʱ��%d\n", time(NULL));

			memcpy(buf, &nID, sizeof(unsigned short));
			int length_s = -1;
			length_s = sendto(extern_sock, buf, length, 0, (struct sockaddr*)&extern_id, sizeof(extern_id));//���͸�DNS������
			if (debug_level == 2)
			//printf("���ⲿ�����꣬��ǰʱ��%d\n", time(NULL));

			if (length_s != -1)printf("�ɹ����͸�DNS������\n");
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
	}//czk�������
	//���ⲿ����

	char recvBuf[MAX_BUF_SIZE];
	memset(recvBuf, 0, MAX_BUF_SIZE);
	int recvlen = -1;
	//if (debug_level == 2)printf("׼�����ⲿ���ܣ���ǰʱ��%d\n", time(NULL));

	setsockopt(extern_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));

	recvlen = recvfrom(extern_sock, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&external, &length_c); /* Receive DNS packet from exterior */
	Record now;
	strcpy(now.ip, inet_ntoa(external.sin_addr));
	//if (debug_level == 2)printf("���ⲿ�����꣺��ǰʱ��%d\n", time(NULL));
	if (recvlen > -1)
	{
		if (debug_level)
		{
			color(6);
			printf("\n\n----���ⲿDNS���յ�[IP:%s]----\n", inet_ntoa(external.sin_addr));

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

		//�ͻ�����Ϣ
		sockaddr_in client_back = ID_Table[id_index].client;

		//����ͻش������
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
			printf("�ⲿDNS������������Ϣ [Url : %s]\n", new_url);

		//������
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
				}//��txt�ļ��ͱ��ز�ѯ���м����²�ѯ����������ر����������
				Record aaa;
				strcpy(aaa.dn, new_url);
				strcpy(aaa.ip, ip);

				Add_To_Cache(aaa);
				//������cache�м�������
				break;
			}
			else p += datalen;  /* If type is not A, then ignore it */
		}

		/* Send packet to client */
		int lensss=-1;
		lensss = sendto(local_sock, recvBuf, recvlen, 0, (SOCKADDR*)&localAddr, sizeof(localAddr));
		if(debug_level){if (lensss != -1)printf("�ɹ����ͣ�����Ϊ%d\n", recvlen); }
		
		color(16);
	}
	else {
		color(4);
		if(debug_level) printf("\nδ�յ��ⲿ��������\nrecvlen=%d\n",recvlen);
		color(16);
	}
	

	return 3;
}





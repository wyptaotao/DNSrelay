#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include<time.h>
#include"def.h"
#include"data.h"
#include <iostream>

void Set_TTL(int ttl,ID_Binding_Unit* a)/*Ϊ�󶨵�λ���ó�ʱʱ��*/ {
	a->dead_time = time(NULL) + ttl;//���ó�ʱʱ��Ϊ��ǰʱ�����������
}

int If_Expired(ID_Binding_Unit a)/*��鵱ǰ�󶨵�λ�Ƿ񳬹��涨ʱ��*/ {
	if (time(NULL) >= a.dead_time) {
		return 1;//����1�����Ѿ���ʱ
	}
	else {
		return 0;//����0����δ��ʱ
	}
}

unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client)/*��һ��ID�󶨵�λ�����ض�Ӧ��ID*/ {
	for (int i = 0; i < 16; i++) {
		if (ID_Table[i].status||If_Expired(ID_Table[i]))/*�����󶨵�λ�Ѿ��ش���ϣ�Ŀǰ���ڿ���״̬�����Ѿ���ʱ*/ {
			ID_Table[i].client = client;//�󶨿ͻ��˵�ַ
			ID_Table[i].prev_ID = ID;//��ԭ�����ݰ�ID
			ID_Table[i].status = 0;//��״̬����Ϊ��δ��ɻش�
			Set_TTL(TTL, &ID_Table[i]);//���ó�ʱʱ��
			if (debug_level == 2) {
				printf("��ID�ɹ������Ϊ %d \n", i);
			}
			return i;
		}
	}
	if (debug_level == 2) {
		printf("��IDʧ��\n");
	}
	return 404;
}

void Transfer_URL(char* asds, char* dest)
{
	int i = 0, j = 0, k = 0, len = strlen(asds);
	while (i < len) 
	{
		if (asds[i] > 0 && asds[i] <= 63)//����λ
		{
			for (j = asds[i], i++; j > 0; j--, i++, k++) 
				dest[k] = asds[i];//��λ����
		}
		if (asds[i] != 0) 
		{
			dest[k] = '.';
			k++;
		}//������λ��ֵΪ"."
	}
	dest[k] = '\0'; //��β
}//�����ݰ��������ĸ�ʽתΪ�����ĵ���ַ���
void Clean_cache(Cache_Unit* a) {
	strcpy_s(a->dn, "");
	for (int i = 0; i < a->ip_count; i++) {
		strcpy_s(a->ip[i], "");//ѭ�����
	}
	a->ip_count = 0;
}//��ջ��浥Ԫ
int Add_To_Cache(Record a) {
	int i;
	for (i = 0; i < MAX_CACHE_SIZE; i++) {
		if (strcmp(a.dn,Cache[i].dn)==0)/*������ͬ*/ {
			strcpy_s(Cache[i].dn, a.dn);
			strcpy_s(Cache[i].ip[Cache[i].ip_count], a.ip);
			Cache[i].ip_count++;
			Cache[i].ttl = CACHE_TTL;//�Ѷ�Ӧ��Ԫ�����ڸ�ֵΪTTL
			//std::cout << "�ɹ���������������" << Cache[i].dn << std::endl;
			return 1;
		}
	}
	for (i = 0; i < MAX_CACHE_SIZE; i++)/**/ {
		if (Cache[i].ttl == 0)/*���������ѵ���*/ {
			strcpy_s(Cache[i].dn, a.dn);
			strcpy_s(Cache[i].ip[Cache[i].ip_count], a.ip);
			Cache[i].ip_count++;
			Cache[i].ttl = CACHE_TTL;//�Ѷ�Ӧ��Ԫ�����ڸ�ֵΪTTL
			//std::cout << "�ɹ�����δ��������" << Cache[i].dn << std::endl;
			return 1;
		}

	}
	return 0;
}//��һ�β�ѯ�������cache������ֵ�����Ƿ�ɹ���1Ϊ�ɹ���0Ϊʧ��
void LFU_Refresh() {
	for (int i = 0; i < MAX_CACHE_SIZE; i++) {
		if (Cache[i].ttl != 0) {
			Cache[i].ttl--;//��һ�μ���
			if (Cache[i].ttl == 0)/*����0*/ {
				Clean_cache(&Cache[i]);//��ոõ�Ԫ
			}
		}//ֻ���·ǿյ�Ԫ
	}
}//LFU�㷨���»���


int Add_To_Txt(char* ip, char* new_url)
{	
	FILE* fp; // �ļ�ָ��
	errno_t err;
	err = fopen_s(&fp, "dnsrelay.txt", "a+"); // ���ļ�

	if (err != 0)
	{
		printf("�ļ���ʧ��!");
		exit(1);
	}
	else
	{
		fprintf(fp, "%\n");
		fputs(ip, fp);
		fputs(" ",fp);
		fputs(new_url, fp);
		fclose(fp);
	}
	return 0;
}
void Add_To_Table(AllRecords *a, char* ip, char* new_url) {
	strcpy_s(a->record[a->count].dn, new_url);
	strcpy_s(a->record[a->count].ip, ip);
	a->count++;
	return;
}

#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include<time.h>
#include"def.h"
#include"data.h"

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
	for (int i = 0; i < MAX_ID_TABLE_SIZE; i++) {
		if (ID_Table[i].status == 1||If_Expired(ID_Table[i]))/*�����󶨵�λ�Ѿ��ش���ϣ�Ŀǰ���ڿ���״̬�����Ѿ���ʱ*/ {
			ID_Table[i].client = client;//�󶨿ͻ��˵�ַ
			ID_Table[i].prev_ID = ID;//��ԭ�����ݰ�ID
			ID_Table[i].status = 0;//��״̬����Ϊ��δ��ɻش�
			Set_TTL(TTL, &ID_Table[i]);//���ó�ʱʱ��
			if (debug_level == 2) {
				printf("��ID�ɹ������Ϊ %d \n",i);
			}
			return i;
		}
	}
	if (debug_level == 2) {
		printf("��IDʧ��\n");
	}
	return -1;
}

void Transfer_URL(char* buf, char* dest)
{
	int i = 0, j = 0, k = 0, len = strlen(buf);
	while (i < len) 
	{
		if (buf[i] > 0 && buf[i] <= 63)//����λ
		{
			for (j = buf[i], i++; j > 0; j--, i++, k++) 
				dest[k] = buf[i];//��λ����
		}
		if (buf[i] != 0) 
		{
			dest[k] = '.';
			k++;
		}//������λ��ֵΪ"."
	}
	dest[k] = '\0'; //��β
}//�����ݰ��������ĸ�ʽתΪ�����ĵ���ַ���
void Clean_cache(Cache_Unit* a) {
	strcpy(a->inf.dn, "nothing");
	strcpy(a->inf.ip, "nothing");//�����ݸ�Ϊ��
}
int Add_To_Cache(Record a) {
	int i;
	for (i = 0; i < MAX_CACHE_SIZE; i++) {
		if (Cache[i].ttl == 0) {
			strcpy(Cache[i].inf.dn, a.dn);
			strcpy(Cache[i].inf.ip, a.ip);
			Cache[i].ttl = TTL;//�Ѷ�Ӧ��Ԫ�����ڸ�ֵΪTTL
		}
		return 1;
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
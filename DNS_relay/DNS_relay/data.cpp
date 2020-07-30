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
			return i;
		}
	}
	printf("��IDʧ��");
	return -1;
}
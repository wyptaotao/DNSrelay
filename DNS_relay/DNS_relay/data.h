#pragma once
#include"def.h"
#include <winsock2.h> 

typedef struct {
	unsigned short prev_ID;//��ͻ���ͨ��ʹ�õ�ID
	int status;//��ǵ�ǰ��Ӧ��ID�Ƿ�Կͻ�����ɻش���0Ϊδ��ɣ�1Ϊ�����
	SOCKADDR_IN client;//�ͻ��˵ĵ�ַ
	int dead_time;//�˴ΰ󶨵�ʧЧʱ��
}ID_Binding_Unit;//��¼��ӦID�Ľṹ��

ID_Binding_Unit ID_Table[MAX_ID_TABLE_SIZE];//ID��Ӧ�󶨱�

void Transfer_URL(char* buf, char* dest);//ת���������ɲ�ѯ��ʽ
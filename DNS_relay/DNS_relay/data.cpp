#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include"def.h"


typedef struct{
	unsigned short prev_ID;//��ͻ���ͨ��ʹ�õ�ID
	int status;//��ǵ�ǰ��Ӧ��ID�Ƿ�Կͻ�����ɻش���0Ϊδ��ɣ�1Ϊ�����
	SOCKADDR_IN client;//�ͻ��˵ĵ�ַ
}ID_Binding_Unit;//��¼��ӦID�Ľṹ��

unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client) {
	for (int i = 0; i < MAX_ID_TABLE_SIZE; i++) {
		if()
	}
}
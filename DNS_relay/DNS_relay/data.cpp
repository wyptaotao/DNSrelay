#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
int

typedef struct {
	unsigned short prev_ID;//��ͻ���ͨ��ʹ�õ�ID
	int status;//��ǵ�ǰ��Ӧ��ID�Ƿ�Կͻ�����ɻش���0Ϊδ��ɣ�1Ϊ�����
	SOCKADDR_IN client;//�ͻ��˵ĵ�ַ
}ID_Binding_Unit;//��¼��ӦID�Ľṹ��

ID_Binding_Unit[];
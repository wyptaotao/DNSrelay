#pragma once
#include"def.h"
#include<string.h>
#include <winsock2.h> 

typedef struct a{
	unsigned short prev_ID;//��ͻ���ͨ��ʹ�õ�ID
	int status;//��ǵ�ǰ��Ӧ��ID�Ƿ�Կͻ�����ɻش���0Ϊδ��ɣ�1Ϊ�����
	SOCKADDR_IN client;//�ͻ��˵ĵ�ַ
	int dead_time;//�˴ΰ󶨵�ʧЧʱ��
}ID_Binding_Unit;//��¼��ӦID�Ľṹ��


extern ID_Binding_Unit ID_Table[MAX_ID_TABLE_SIZE];//ID��Ӧ�󶨱�
extern Cache_Unit Cache[MAX_CACHE_SIZE];//�����


void Transfer_URL(char* buf, char* dest);//ת���������ɲ�ѯ��ʽ
int If_Expired(ID_Binding_Unit a);//��ѯת����ѯ�Ƿ�ʱ
void Set_TTL(int ttl, ID_Binding_Unit* a);//����ת����ѯ����ʱ��
unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client);//��ת����ѯ����id����������id
int Add_To_Cache(Record a);
void Clean_cache(Cache_Unit* a);
void LFU_Refresh();
int Add_To_Txt(char* ip, char* new_url);
void Add_To_Table(AllRecords* a, char* ip, char* new_url);
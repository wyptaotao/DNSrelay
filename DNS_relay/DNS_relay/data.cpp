#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include"def.h"
#include"data.h"

unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client) {
	for (int i = 0; i < MAX_ID_TABLE_SIZE; i++) {
		if (ID_Table[i].status == 1)/*�����󶨵�λ�Ѿ��ش���ϣ�Ŀǰ���ڿ���״̬*/ {

		}
	}
}
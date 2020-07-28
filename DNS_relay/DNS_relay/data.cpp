#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include"def.h"


typedef struct{
	unsigned short prev_ID;//与客户端通信使用的ID
	int status;//标记当前对应的ID是否对客户端完成回传，0为未完成，1为已完成
	SOCKADDR_IN client;//客户端的地址
}ID_Binding_Unit;//记录对应ID的结构体

unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client) {
	for (int i = 0; i < MAX_ID_TABLE_SIZE; i++) {
		if()
	}
}
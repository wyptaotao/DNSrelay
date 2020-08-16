#pragma once
#include"def.h"
#include <winsock2.h> 

typedef struct {
	unsigned short prev_ID;//与客户端通信使用的ID
	int status;//标记当前对应的ID是否对客户端完成回传，0为未完成，1为已完成
	SOCKADDR_IN client;//客户端的地址
	int dead_time;//此次绑定的失效时间
}ID_Binding_Unit;//记录对应ID的结构体

ID_Binding_Unit ID_Table[MAX_ID_TABLE_SIZE];//ID对应绑定表

void Transfer_URL(char* buf, char* dest);//转换域名至可查询格式
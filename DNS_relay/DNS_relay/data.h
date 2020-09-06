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
int If_Expired(ID_Binding_Unit a);//查询转发查询是否超时
void Set_TTL(int ttl, ID_Binding_Unit* a);//设置转发查询生存时间
unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client);//给转发查询绑定新id，并返回其id
void Transfer_URL(char* buf, char* dest);//将数据包中域名的格式转为正常的点分字符串
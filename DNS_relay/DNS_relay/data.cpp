#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include<time.h>
#include"def.h"
#include"data.h"

void Set_TTL(int ttl,ID_Binding_Unit* a)/*为绑定单位设置超时时间*/ {
	a->dead_time = time(NULL) + ttl;//设置超时时间为当前时间加上生存期
}

int If_Expired(ID_Binding_Unit a)/*检查当前绑定单位是否超过规定时间*/ {
	if (time(NULL) >= a.dead_time) {
		return 1;//返回1代表已经超时
	}
	else {
		return 0;//返回0代表还未超时
	}
}

unsigned short Bind_ID(unsigned short ID, SOCKADDR_IN client)/*绑定一个ID绑定单位，返回对应的ID*/ {
	for (int i = 0; i < MAX_ID_TABLE_SIZE; i++) {
		if (ID_Table[i].status == 1||If_Expired(ID_Table[i]))/*该条绑定单位已经回传完毕，目前处于空闲状态或者已经超时*/ {
			ID_Table[i].client = client;//绑定客户端地址
			ID_Table[i].prev_ID = ID;//绑定原来数据包ID
			ID_Table[i].status = 0;//将状态设置为还未完成回传
			Set_TTL(TTL, &ID_Table[i]);//设置超时时间
			return i;
		}
	}
	printf("绑定ID失败");
	return -1;
}
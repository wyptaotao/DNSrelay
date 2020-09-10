#include<stdio.h>
#include<string.h>
#include <winsock2.h> 
#include<time.h>
#include"def.h"
#include"data.h"
#include <iostream>

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
	for (int i = 0; i < 16; i++) {
		if (ID_Table[i].status||If_Expired(ID_Table[i]))/*该条绑定单位已经回传完毕，目前处于空闲状态或者已经超时*/ {
			ID_Table[i].client = client;//绑定客户端地址
			ID_Table[i].prev_ID = ID;//绑定原来数据包ID
			ID_Table[i].status = 0;//将状态设置为还未完成回传
			Set_TTL(TTL, &ID_Table[i]);//设置超时时间
			if (debug_level == 2) {
				printf("绑定ID成功！编号为 %d \n", i);
			}
			return i;
		}
	}
	if (debug_level == 2) {
		printf("绑定ID失败\n");
	}
	return 404;
}

void Transfer_URL(char* asds, char* dest)
{
	int i = 0, j = 0, k = 0, len = strlen(asds);
	while (i < len) 
	{
		if (asds[i] > 0 && asds[i] <= 63)//计数位
		{
			for (j = asds[i], i++; j > 0; j--, i++, k++) 
				dest[k] = asds[i];//逐位复制
		}
		if (asds[i] != 0) 
		{
			dest[k] = '.';
			k++;
		}//将计数位赋值为"."
	}
	dest[k] = '\0'; //结尾
}//将数据包中域名的格式转为正常的点分字符串
void Clean_cache(Cache_Unit* a) {
	strcpy_s(a->dn, "");
	for (int i = 0; i < a->ip_count; i++) {
		strcpy_s(a->ip[i], "");//循环清空
	}
	a->ip_count = 0;
}//清空缓存单元
int Add_To_Cache(Record a) {
	int i;
	for (i = 0; i < MAX_CACHE_SIZE; i++) {
		if (strcmp(a.dn,Cache[i].dn)==0)/*域名相同*/ {
			strcpy_s(Cache[i].dn, a.dn);
			strcpy_s(Cache[i].ip[Cache[i].ip_count], a.ip);
			Cache[i].ip_count++;
			Cache[i].ttl = CACHE_TTL;//把对应单元生存期赋值为TTL
			//std::cout << "成功加入已有域名：" << Cache[i].dn << std::endl;
			return 1;
		}
	}
	for (i = 0; i < MAX_CACHE_SIZE; i++)/**/ {
		if (Cache[i].ttl == 0)/*生命周期已到的*/ {
			strcpy_s(Cache[i].dn, a.dn);
			strcpy_s(Cache[i].ip[Cache[i].ip_count], a.ip);
			Cache[i].ip_count++;
			Cache[i].ttl = CACHE_TTL;//把对应单元生存期赋值为TTL
			//std::cout << "成功加入未有域名：" << Cache[i].dn << std::endl;
			return 1;
		}

	}
	return 0;
}//将一次查询结果放入cache，返回值代表是否成功，1为成功，0为失败
void LFU_Refresh() {
	for (int i = 0; i < MAX_CACHE_SIZE; i++) {
		if (Cache[i].ttl != 0) {
			Cache[i].ttl--;//减一次计数
			if (Cache[i].ttl == 0)/*减到0*/ {
				Clean_cache(&Cache[i]);//清空该单元
			}
		}//只更新非空单元
	}
}//LFU算法更新缓存


int Add_To_Txt(char* ip, char* new_url)
{	
	FILE* fp; // 文件指针
	errno_t err;
	err = fopen_s(&fp, "dnsrelay.txt", "a+"); // 打开文件

	if (err != 0)
	{
		printf("文件打开失败!");
		exit(1);
	}
	else
	{
		fprintf(fp, "%\n");
		fputs(ip, fp);
		fputs(" ",fp);
		fputs(new_url, fp);
		fclose(fp);
	}
	return 0;
}
void Add_To_Table(AllRecords *a, char* ip, char* new_url) {
	strcpy_s(a->record[a->count].dn, new_url);
	strcpy_s(a->record[a->count].ip, ip);
	a->count++;
	return;
}

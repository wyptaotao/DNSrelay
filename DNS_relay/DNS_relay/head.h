#include <winsock2.h> 
#include <time.h> 
#include <fstream>
#include <string>
#include <map>
#include <list>
using namespace std;
#pragma warning(disable:4996)

#define MAX_BUF_SIZE 1024          /* Max buffer size */
#define DNS_PORT 53                /* DNS port */
#define ID_TRANS_TABLE_SIZE 16 /* MAX size of transfer table */
#define ID_EXPIRE_TIME 10          /* Expired time is 10s*/
#define MAX_CACHE_SIZE 5           /* Max size of cache */
#define DNS_HEAD_SIZE 12

/* Unit of ID transfer table */
typedef struct
{
	unsigned short old_ID; /* The old ID */
	BOOL done;             /* Mark whether the request was analysed completely */
	SOCKADDR_IN client;    /* Requestor socket address */
	int expire_time;       /* Expire time */
}ID_Trans_Unit;

int debug_level = 0; /* Debug level */
char DNS_Server_IP[16] = "10.3.9.4"; /* Extern DNS server IP default value */

ID_Trans_Unit ID_Trans_Table[ID_TRANS_TABLE_SIZE];	/* ID transfer table */
int ID_Count = 0; /* Size of ID transfer table */

WSADATA wsaData;  /* Store Windows Sockets initialization information */
SOCKET local_sock, extern_sock; /* Local socket and extern socket */

struct sockaddr_in extern_id;//AF_INETµÿ÷∑
struct sockaddr_in client, external;
int length_client = sizeof(client);

map<string, string> local_table;   /* Storage local data */
map<string, string> cache;         /* Cache */
list<string> cache_LRU_list;       /* Storage Url list for LRU algorithm */

/* Output cache table */
void output_cache()
{
	printf("\n\n--------------  Cache  --------------\n");
	int j = 0;
	for (map<string, string>::iterator i = cache.begin(); i != cache.end(); i++)
	{
		printf("#%d Url:%s -> IP:%s\n", j++, i->first.c_str(), i->second.c_str());
	}
	printf("-------------- LRU list--------------\n");
	for (list<string>::iterator i = cache_LRU_list.begin(); i != cache_LRU_list.end(); i++)
	{
		if (i != cache_LRU_list.begin())printf("->");
		printf("%s\n", i->c_str());
	}
}

/* Output at the beginning */
void disp_head()
{
	printf("***********************************************************\n");
	printf("* @Course Name: Course Design of Computer Network         *\n");
	printf("* @Name of Team members: Chen Bin, Luo Yang, Li Yiyang    *\n");
	printf("* @Teacher: Wu Qifan         @Class number: 2017211318    *\n");
	printf("* ------------------------------------------------------- *\n");
	printf("*               DNS Relay Server - Ver 1.0                *\n");
	printf("***********************************************************\n");
	printf("Command syntax : dnsrelay [-d | -dd] [dns-server-IP-addr]  \n");
}

/* Read info from local data */
void Read_Local_Data()
{
	FILE* file;
	if ((file = fopen("dnsrelay.txt", "r")) == NULL) /* No such file then return */
		return;
	char url[65], ip[16];
	while (fscanf(file, "%s %s", ip, url) > 0)
	{
		if (debug_level >= 1)
			printf("Read from 'dnsrelay.txt' -> [Url : %s, IP : %s]\n", url, ip);
		local_table[url] = ip;
	}
	fclose(file);
}

/* Add new record to cache */
void Add_Record_to_Cache(string url, string ip)
{
	map<string, string>::iterator i = cache.find(url);
	if (i != cache.end()) /* The record is already in cache */
	{
		i->second = ip; /* Update IP */
	}
	else /* The record is not in cache */
	{
		if (cache.size() >= MAX_CACHE_SIZE) /* Cache is full (Use LRU algorithm) */
		{
			string old_url = cache_LRU_list.back(); /* Get the last Url */
			cache.erase(old_url); /* Delete the old record in cache */
			cache_LRU_list.pop_back(); /* Delete the old record in LRU list */
		}
		/* Add new record */
		cache[url] = ip;
		cache_LRU_list.push_front(url);
		output_cache();
	}
}

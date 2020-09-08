#include <stdio.h>
#include <winsock2.h>
#include "def.h"
#pragma warning(disable:4996)

#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#define BUF_SIZE 100
extern char ip[2000][20];
extern char dn[2000][100];//domain name
extern SOCKET local_sock;
extern sockaddr_in localAddr;
extern AllRecords r;
//extern SOCKADDR clntAddr;
extern int count;
extern char DNS_Server_IP[16];
extern char file_open[30];
void pre_print()
{
    printf("****************************************************************\n");
    printf("* @Course Name: Course Design of Computer Network              *\n");
    printf("* @Name of Team members: Wang Yupeng, Cheng Zekai, Chen Shuyi  *\n");
    printf("* @Teacher: Cheng Li         @Class number: 2018211314         *\n");
    printf("* -------------------------------------------------------------*\n");
    printf("*               DNS Relay Server - Ver 1.0                     *\n");
    printf("* Command syntax : dnsrelay [-d | -dd] [dns-server-IP-addr]    *\n");
    printf("****************************************************************\n");
}

void init_sock()
{
    /* socket编程要调用各种socket函数，但是需要库Ws2_32.lib和头文件Winsock2.h，这里的WSAStartup就是为了向操作系统说明，
    我们要用哪个库文件，让该库文件与当前的应用程序绑定，从而就可以调用该版本的socket的各种函数了*/
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    local_sock = socket(AF_INET, SOCK_DGRAM, 0);
    //SOCKET extern_sock = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定套接字
    memset(&localAddr, 0, sizeof(localAddr));   //每个字节都用0填充
    localAddr.sin_family = AF_INET;             //使用IPv4地址
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址,在这种情况下，可以收到发送到本机（服务器端）所有有效地址上数据包。
    localAddr.sin_port = htons(53);                //UDP端口

    if (bind(local_sock, (SOCKADDR*)&localAddr, sizeof(SOCKADDR)) == 0)//bind()函数通过给一个未命名套接口分配一个本地名字来为套接口建立本地捆绑（主机地址/端口号）。
    {
        printf("端口绑定成功\n");
    }
    else
    {
        printf("端口绑定失败\n");
        exit(1);//强制退出程序
    }
    extern_sock = socket(AF_INET, SOCK_DGRAM, 0);
    extern_id.sin_family = AF_INET;                         /* Set the family as AF_INET (TCP/IP) */
    extern_id.sin_addr.s_addr = inet_addr(DNS_Server_IP);   /* Set to the IP of extern DNS server */
    extern_id.sin_port = htons(DNS_PORT);                   /* Set the port as DNS port (53) */
}

int init_data()
{
    int i = 0;
    FILE* fp; // 文件指针
    errno_t err;
    //加一点代码
    if (debug_level != 1) {
        strcpy(file_open, "dnsrelay.txt");
    }
    //
    err = fopen_s(&fp, file_open, "r"); // 打开文件
    if (err != 0)
    {
        printf("文件打开失败!");
        exit(1);
    }
    while (!feof(fp)) {
        fscanf_s(fp, "%s %s", r.record[i].ip, _countof(r.record[i].ip), r.record[i].dn, _countof(r.record[i].dn));
        printf("IP是:%s 对应域名:%s\n", r.record[i].ip, r.record[i].dn);
        i++;
    }
    r.count = i;
    printf("总记录数:%d\n", r.count);
    fclose(fp); // 关闭文件指针
}


//int main()
//{
//    init_data();
//    return 0;
//}
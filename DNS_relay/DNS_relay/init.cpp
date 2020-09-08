#include <stdio.h>
#include <winsock2.h>
#include "def.h"
#pragma warning(disable:4996)

#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
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
    /* socket���Ҫ���ø���socket������������Ҫ��Ws2_32.lib��ͷ�ļ�Winsock2.h�������WSAStartup����Ϊ�������ϵͳ˵����
    ����Ҫ���ĸ����ļ����øÿ��ļ��뵱ǰ��Ӧ�ó���󶨣��Ӷ��Ϳ��Ե��øð汾��socket�ĸ��ֺ�����*/
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //�����׽���
    local_sock = socket(AF_INET, SOCK_DGRAM, 0);
    //SOCKET extern_sock = socket(AF_INET, SOCK_DGRAM, 0);

    //���׽���
    memset(&localAddr, 0, sizeof(localAddr));   //ÿ���ֽڶ���0���
    localAddr.sin_family = AF_INET;             //ʹ��IPv4��ַ
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY); //�Զ���ȡIP��ַ,����������£������յ����͵��������������ˣ�������Ч��ַ�����ݰ���
    localAddr.sin_port = htons(53);                //UDP�˿�

    if (bind(local_sock, (SOCKADDR*)&localAddr, sizeof(SOCKADDR)) == 0)//bind()����ͨ����һ��δ�����׽ӿڷ���һ������������Ϊ�׽ӿڽ�����������������ַ/�˿ںţ���
    {
        printf("�˿ڰ󶨳ɹ�\n");
    }
    else
    {
        printf("�˿ڰ�ʧ��\n");
        exit(1);//ǿ���˳�����
    }
    extern_sock = socket(AF_INET, SOCK_DGRAM, 0);
    extern_id.sin_family = AF_INET;                         /* Set the family as AF_INET (TCP/IP) */
    extern_id.sin_addr.s_addr = inet_addr(DNS_Server_IP);   /* Set to the IP of extern DNS server */
    extern_id.sin_port = htons(DNS_PORT);                   /* Set the port as DNS port (53) */
}

int init_data()
{
    int i = 0;
    FILE* fp; // �ļ�ָ��
    errno_t err;
    //��һ�����
    if (debug_level != 1) {
        strcpy(file_open, "dnsrelay.txt");
    }
    //
    err = fopen_s(&fp, file_open, "r"); // ���ļ�
    if (err != 0)
    {
        printf("�ļ���ʧ��!");
        exit(1);
    }
    while (!feof(fp)) {
        fscanf_s(fp, "%s %s", r.record[i].ip, _countof(r.record[i].ip), r.record[i].dn, _countof(r.record[i].dn));
        printf("IP��:%s ��Ӧ����:%s\n", r.record[i].ip, r.record[i].dn);
        i++;
    }
    r.count = i;
    printf("�ܼ�¼��:%d\n", r.count);
    fclose(fp); // �ر��ļ�ָ��
}


//int main()
//{
//    init_data();
//    return 0;
//}
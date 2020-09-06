#include <stdio.h>
#include <winsock2.h>
#include "def.h"
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#define BUF_SIZE 100
extern char ip[2000][20];
extern char dn[2000][100];//domain name
extern SOCKET local_sock;
extern sockaddr_in localAddr;
extern AllRecords r;
//extern SOCKADDR clntAddr;
extern int count;
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
        printf("bind socket port successfully.");
    }
    else
    {
        printf("bind socker port failed.");
        exit(1);//ǿ���˳�����
    }
}

int init_data()
{
    int i = 0;
    FILE* fp; // �ļ�ָ��
    errno_t err;
    err = fopen_s(&fp, "dnsrelay.txt", "r"); // ���ļ�
    if (err != 0)
    {
        printf("The file hasn't been opened.");
        exit(1);
    }
    while (!feof(fp)) {
        fscanf_s(fp, "%s %s", r.record[i].ip, _countof(r.record[i].ip), r.record[i].dn, _countof(r.record[i].dn));
        printf("ip:%s domain name:%s\n", r.record[i].ip, r.record[i].dn);
        i++;
    }
    r.count = i;
    printf("total record number:%d\n", r.count);
    fclose(fp); // �ر��ļ�ָ��
}

//int main()
//{
//    init_data();
//    return 0;
//}
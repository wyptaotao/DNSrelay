#include<iostream>
#include "def.h"
#include <winsock2.h>

extern SOCKET local_sock;
extern SOCKADDR clntAddr;
void query()
{
	char buffer[MAX_BUF_SIZE] = { 0 };
	int nSize = sizeof(SOCKADDR);
	int strLen = recvfrom(local_sock, buffer, sizeof(buffer), 0, &clntAddr, &nSize);

	printf("the strLen is : %d\n", strLen);
	for (int i = 0; i < strLen; i++)
	{
		printf("@@@@@@@@@ %c @@@@@@@@@@@@@\n", buffer[i]);//�ӵ�ע��
	}
	printf("\n@@@@     end      @@@@\n");

	//if (/*retΪ�ӿͻ����յ���*/)//Ϊ��ѯ����
	//{
	//	//�ȴӱ��س����￪ʼ��
	//	if (/*�ӱ��س�����鵽*/)
	//	{
	//		//�ǵ��жϲ鵽��IP��ַ�ǲ���0.0.0.0
	//		//sento();�ط����ͻ���
	//	}
	//	else///*�ӱ��س�����û�鵽*/
	//	{
	//		//��ID�󶨵�Ԫ
	//		//���ݰ󶨺�ķ��ص�ID���޸İ������ݣ��޸�id��ɶ�ģ�
	//		//���͸�ʵ�ʵ�DNS������
	//	}
	//}

	//if (/*retΪ��ʵ�ʵķ��������յ��İ�*/)//Ϊ�м̵Ļش�
	//{

	//	//�ж��յ������������Ҫ��ѯ���Ƿ���ͬ����ͬ����������ͬ�����
	//	//һЩ�������ҵ���IDɶ�Ĺ���û��Ͱ�
	//	//���͸��ͻ���
	//}
}

#include<time.h>
#include<stdio.h>

void print_time()
{
	struct tm t;   //tm�ṹָ��
	time_t now;  //����time_t���ͱ���
	time(&now);      //��ȡϵͳ���ں�ʱ��
	localtime_s(&t, &now);   //��ȡ�������ں�ʱ��

	   //��ʽ���������ʱ��
	printf("�꣺%d\n", t.tm_year + 1900);
	printf("�£�%d\n", t.tm_mon + 1);
	printf("�գ�%d\n", t.tm_mday);
	printf("ʱ��%d\n", t.tm_hour);
	printf("�֣�%d\n", t.tm_min);
	printf("�룺%d\n", t.tm_sec);
}

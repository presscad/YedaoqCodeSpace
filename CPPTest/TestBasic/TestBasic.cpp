// TestBasic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <winbase.h>
#include <string.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <mbstring.h>

#include "test.h"

using namespace std;

void test1()
{
	printf("Hello World!\n");
	char buffer[10] = "����abc";
	printf("%s len : %d\r\n",buffer,strlen(buffer));
	printf("%s len : %d\r\n",buffer,_mbslen((const unsigned char *)buffer));

	// Conclusion : �������ַ����������ֽ��ַ������룬����Lǰ׺���򽫱���ΪUnicode�ַ�����
}

void test2()
{
	int i = 5;

	char *str = (char*)(&i);

	cout<< "(str = i) : "<< str << endl;

	char c = i;
	
	cout<< "(c = i) : "<< c << endl;

	c = (char)i;
	
	cout<< "(c = (char)i) : "<< c << endl;

	c = static_cast<char>(i);

	cout<< "(c = static_cast<char>(i)) : "<< c << endl;

	i = c;

	cout<< "(i = c) : " << i <<endl;

	i = static_cast<int>(i);
	
	cout<< "(i = static_cast<int>(c)) : "<< i << endl;

	// Conclusion : Intel �ֽ���Ϊ LITTLE-ENDIAN
	// Conclusion : static_cast �Ĺ����� ǿ��ת�� ��ͬ����static_castֻ������ͬһ����ϵ�����ͼ�ת��
	// Conclusion : �����������ε�ǿ��ת��������Ҫͨ��atoi�Ⱥ�����ʵ��
}

int main1(int argc, char* argv[])
{
	test3();

	return 0;
}


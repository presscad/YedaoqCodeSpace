//���ۣ���������������ͳ�Ա�������ݳ�Աʱ���������ܹ���
//      1. ����Ĭ�Ͽ������캯�����������Զ��������ͳ�Ա�ͳ������ݳ�Ա�ĳ�ʼ����
//      2. ������Ĭ�ϸ�ֵ�����

#include "StdAfx.h"
#include <string>

using namespace std;

template<class T>
class NamedObject
{
	public:
		NamedObject(std::string& name, const T& val):nameValue(name),objValue(val){}

	private:
		std::string& nameValue;
		const T objValue;
};

int mainRMACD()
{
	std::string strA("strA");
	std::string strB("strB");
	NamedObject<int> objA(strA,2);
	NamedObject<int> objB(strB,3);
	NamedObject<int> objC(objA);
	//objB = objA;
	return 1;
}
/* ___________________________________________________________________________
@ 
@ file - CAtlComModuleScope.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-2-7
@ info -
@        ��Scope�������������ṩATL�е�COM����
@ -----------
@ ���� : ��ʼ��ȫ�ֶ���ATL::_pModule��ATL::_pAtlModule��������AtlAxWinInitע��AtlAxWinx������
@ Ҫ�� : 
@    1. ʹ�þ�̬���ü�����������Scope�����ڼ�ά��COM���������Ҳ��ظ���ʼ��
@    2. CComModule��CAtlModule���乹�캯��������ATL::_pModule��ATL::_pAtlModuleȫ�ֱ���������ζ�ţ�
@         a. ���û�ͨ��������ʽ������Module����Ļ�������಻Ӧ���ٴ���Module�������⸲�ǣ�
@         b. �����ഴ��Module����֮���û�����������ʽ����Module����Ļ���ATL::_pModule������ָ
@            ���ഴ����Moduleʵ������������ܹ�ʶ�������������Ӧ��ֻ�����Լ��������Ǹ�ʵ����
@            Ҳ����˵����ֱ������ATL::_pModuleָ��Ķ��󡣱��磬�����ഴ���Ķ����ڶ��ϣ����û���
@            ջ�ϴ�������һ��Module���󲢽�ATL::_pModuleָ����������ɾ�����������쳣��
@       ��ˣ���������ü����⣬����ά��һ����̬Moduleʵ��ָ�룬�����ٽ��Աȴ�ָ����ATL::_pModule
@       ��ֵ����������һ��ʱ�����ATL::_pModuleָ��Ķ���
/* ___________________________________________________________________________*/

#pragma once
#include <atlbase.h>
#include <atlwin.h>

class CAtlComModuleScope
{
public:
	CAtlComModuleScope();
	~CAtlComModuleScope();

protected:
	static unsigned int		RefCount;
	static ATL::CComModule* InstancePtr;
};

__declspec(selectany) unsigned int CAtlComModuleScope::RefCount = 0;
__declspec(selectany) ATL::CComModule* CAtlComModuleScope::InstancePtr;

CAtlComModuleScope::CAtlComModuleScope()
{
	if(0 == RefCount)
	{
		if(!ATL::_pModule)
		{
			InstancePtr = ATL::_pModule = new ATL::CComModule();
		}
		AtlAxWinInit();
	}
	++RefCount; //InterlockedIncrement(RefCount);
}

CAtlComModuleScope::~CAtlComModuleScope()
{								
	if(1 == RefCount)					// ������ֵΪ1ʱ���������λ�����Ƿ�����ATL::pModule
	{
		if(InstancePtr == ATL::_pModule && InstancePtr)
		{
			ATL::CComModule* pTemp = InstancePtr;
			ATL::_pAtlModule = 0;
			InstancePtr = ATL::_pModule = 0;			
			delete pTemp;
		}

		AtlAxWinTerm();
	}
	else if(0 == RefCount)				// �����
	{
		return;
	}								
	--RefCount;							// �ݼ�����ֵ
}

/* ___________________________________________________________________________
@ 
@ file - OcxFile.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-2-7
@ info -
@     ���ڸ�������ActiveX�ؼ����ɴ���δע�����
@	  ָ��·��ʱ����Ϊδע����������������������Ϊ��ע���������
@	  CAxWindow�ɸ���ProgID��CLSID��Name������������������֧��CISID
/* ___________________________________________________________________________*/

#pragma once
#include "ComFile.h"
#include "CAtlComModuleScope.h"

class COcxFile : public CComFile
{
public:
	COcxFile(REFCLSID clsid, LPCTSTR lpOcxPath = 0, LPCTSTR lpLicence = 0)
		: CComFile(clsid, lpOcxPath, lpLicence)
	{}

protected:
	COcxFile& operator=(const COcxFile&);

public:

	HRESULT CreateControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

	HRESULT CreateUnregisteredControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

	HRESULT CreateRegisteredControl(ATL::CAxWindow& pTarget, HWND hParent, ATL::_U_RECT rect = NULL
		, DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);

protected:
	CAtlComModuleScope AtlAxWinInitScope;
};

#include "stdafx.h" 
#include "WMI_MACAddress.h" 
#include <comutil.h> 
#include <Wbemidl.h> 
#include <tchar.h> 
#include <strsafe.h> 
#include <algorithm> 
#include <ntddndis.h> 
#include <atlconv.h> 
#include <WinIoCtl.h>
#include <objbase.h>

#pragma comment (lib, "ole32.lib")
#pragma comment (lib, "comsuppw.lib") 
#pragma comment (lib, "wbemuuid.lib") 

typedef struct _T_WQL_QUERY 
{ 
	CHAR*	szSelect;		// SELECT��� 
	WCHAR*	szProperty;		// �����ֶ� 
} T_WQL_QUERY; 

// WQL��ѯ��� 
const T_WQL_QUERY szWQLQuery[] = { 
	// ����USB���� 
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", 
	L"PNPDeviceID", 

	// ������USB���� 
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%')) AND (NOT (PNPDeviceID LIKE 'USB%'))", 
	L"PNPDeviceID" 
}; 

static BOOL WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, T_MACADDRESS *pMacAddress, INT iIndex ) 
{ 
	TCHAR	DevicePath[MAX_PATH]; 
	HANDLE	hDeviceFile;	 
	BOOL	isOK = FALSE; 

	// �����豸·���� 
	StringCchCopy( DevicePath, MAX_PATH, TEXT("\\\\.\\") ); 
	StringCchCat( DevicePath, MAX_PATH, PNPDeviceID ); 
	StringCchCat( DevicePath, MAX_PATH, TEXT("#{ad498944-762f-11d0-8dcb-00c04fc3358c}") ); 

	// ����PNPDeviceID���еġ�\���滻�ɡ�#�����Ի���������豸·���� 
	std::replace( DevicePath + 4, DevicePath + 4 + _tcslen(PNPDeviceID), TEXT('\\'), TEXT('#') );  

	// ��ȡ�豸��� 
	hDeviceFile = CreateFile( DevicePath, 
 		0, 
 		FILE_SHARE_READ | FILE_SHARE_WRITE, 
 		NULL, 
 		OPEN_EXISTING, 
 		0, 
 		NULL); 

	if( hDeviceFile != INVALID_HANDLE_VALUE ) 
	{	 
 		ULONG	dwID; 
 		BYTE	ucData[8]; 
 		DWORD	dwByteRet;		 

 		// ��ȡ��ǰMAC��ַ 
 		dwID = OID_802_3_CURRENT_ADDRESS; 
 		isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL ); 
 		if( isOK ) 
 		{	 
 			memcpy( pMacAddress[iIndex].MACAddress, ucData, dwByteRet ); 

 			// ��ȡԭ��MAC��ַ 
 			dwID = OID_802_3_PERMANENT_ADDRESS; 
 			isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL ); 
 			if( isOK ) 
 			{	 
 				memcpy( pMacAddress[iIndex].PermanentAddress, ucData, dwByteRet ); 
 			} 
 		} 

 		CloseHandle( hDeviceFile ); 
	} 

	return isOK; 
} 

INT WMI_MacAddress( INT iQueryType, T_MACADDRESS *pMacAddress, INT iSize ) 
{ 
	HRESULT hres; 
	INT	iTotal = 0; 
 
	// �жϲ�ѯ�����Ƿ�֧�� 
	if( (iQueryType  < 0) || (iQueryType >= sizeof(szWQLQuery)/sizeof(T_WQL_QUERY)) ) 
	{ 
 		return -1;	// ��ѯ���Ͳ�֧�� 
	} 

	// ��ʼ��COM 
	 hres = CoInitializeEx( NULL, COINIT_MULTITHREADED );  
	 if( FAILED(hres) ) 
	 { 
		 return -2; 
	 } 

	 // ����COM�İ�ȫ��֤���� 
	hres = CoInitializeSecurity(  
 		NULL,  
 		-1,  
 		NULL,  
 		NULL,  
 		RPC_C_AUTHN_LEVEL_DEFAULT,  
 		RPC_C_IMP_LEVEL_IMPERSONATE, 
		NULL, 
		EOAC_NONE, 
		NULL 
 		); 
	if( FAILED(hres) ) 
	 { 
		 CoUninitialize(); 
		 return -2; 
	 } 
  
	// ���WMI����COM�ӿ� 
	 IWbemLocator *pLoc = NULL; 
	 hres = CoCreateInstance(  
 		CLSID_WbemLocator,              
		 NULL,  
		 CLSCTX_INPROC_SERVER,  
		 IID_IWbemLocator, 
 		reinterpret_cast <LPVOID*>(&pLoc) 
 		);  
	 if( FAILED(hres) ) 
	 { 
 		CoUninitialize(); 
		 return -2; 
	 } 

	 // ͨ�����ӽӿ�����WMI���ں˶�����"ROOT\\CIMV2" 
	IWbemServices *pSvc = NULL; 
	hres = pLoc->ConnectServer( 
		  _bstr_t( L"ROOT\\CIMV2" ), 
		  NULL, 
		  NULL, 
		  NULL, 
		  0, 
		  NULL, 
		  NULL, 
		  &pSvc 
 		 );     
	 if( FAILED(hres) ) 
	 { 
 		pLoc->Release();  
		 CoUninitialize(); 
		 return -2; 
	 } 

	// �����������İ�ȫ���� 
	 hres = CoSetProxyBlanket( 
 		pSvc, 
 		RPC_C_AUTHN_WINNT, 
 		RPC_C_AUTHZ_NONE, 
 		NULL, 
 		RPC_C_AUTHN_LEVEL_CALL, 
 		RPC_C_IMP_LEVEL_IMPERSONATE, 
 		NULL, 
 		EOAC_NONE 
 		); 
	if( FAILED(hres) ) 
	 { 
		 pSvc->Release(); 
		 pLoc->Release();      
		 CoUninitialize(); 
		 return -2; 
	 } 

	 // ͨ�������������WMI�������� 
	 IEnumWbemClassObject *pEnumerator = NULL; 
	 hres = pSvc->ExecQuery( 
 		bstr_t("WQL"),  
 		bstr_t( szWQLQuery[iQueryType].szSelect ), 
		 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,  
		 NULL, 
		 &pEnumerator 
 		); 
	if( FAILED(hres) ) 
	 { 
		 pSvc->Release(); 
		 pLoc->Release(); 
		 CoUninitialize(); 
		 return -3; 
	 } 

	 // ѭ��ö�����еĽ������   
	 while( pEnumerator ) 
	 { 
 		IWbemClassObject *pclsObj = NULL; 
 		ULONG uReturn = 0; 

 		if( (pMacAddress != NULL) && (iTotal >= iSize) ) 
 		{ 
 			break; 
 		} 

		 pEnumerator->Next( 
 			WBEM_INFINITE, 
 			1,  
			 &pclsObj, 
 			&uReturn 
 			); 

		 if( uReturn == 0 ) 
		 { 
			 break; 
		 } 

 		VARIANT	vtProperty; 
 		TCHAR szProperty[128]; 
 			 
 		// ��ȡ�����豸��ʶ�� 
 		VariantInit( &vtProperty );	 
 		pclsObj->Get( szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL ); 
		USES_CONVERSION; 
 		StringCchCopy( szProperty, sizeof(szProperty)/sizeof(TCHAR), W2T(vtProperty.bstrVal) ); 
 		VariantClear( &vtProperty ); 

 		if( pMacAddress != NULL ) 
 		{	// ͨ���豸��ʶ����ȡԭ��MAC��ַ�͵�ǰMAC��ַ 
 			if( WMI_DoWithPNPDeviceID( szProperty, pMacAddress, iTotal ) ) 
 			{ 
 				iTotal++; 
 			} 
 		} 
 		else 
 		{ 
 			iTotal++; 
 		} 

 		pclsObj->Release(); 
	 } // End While 

	 // �ͷ���Դ 
	pEnumerator->Release(); 
	 pSvc->Release(); 
	 pLoc->Release();     
	 CoUninitialize(); 

	 return iTotal; 
} 
/* ----------------------------------------------------------   
�ļ����ƣ�WMI_MACAddress.h   

  ���ߣ��ؽ���   
  
	MSN��splashcn@msn.com   
	
	  �汾��ʷ��   
	  V1.1    2010��05��23��   
	  �����Ż���   
	  
		V1.0    2010��05��12��   
		�����ʽ�汾��   
		
		  ����������   
		  ���WMI��DeviceIoControl��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ   
		  
			�ӿں�����   
			WMI_MacAddress   
------------------------------------------------------------ */   
#pragma once    

#include  <windows.h>    

#define MACADDRESS_BYTELEN      6   // MAC��ַ�ֽڳ���    

typedef struct _T_MACADDRESS {    
	BYTE    PermanentAddress[MACADDRESS_BYTELEN];   // ԭ��MAC��ַ    
	BYTE    MACAddress[MACADDRESS_BYTELEN];         // ��ǰMAC��ַ    
} T_MACADDRESS;    

//-------------------��������-------------    
#ifdef __cplusplus    
extern "C"{    
#endif    
    
/*   
���ܣ����WMI��DeviceIoControl��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ   
��ڲ�����   
iQueryType����Ҫ��ȡ����������   
0������USB����   
1��������USB����   
pMacAddress���洢����MAC��ַ   
uSize���ɴ洢�����������Ŀ   
����ֵ��   
-1����֧�ֵ��豸����ֵ   
-2��WMI����ʧ��   
-3������ȷ��WQL��ѯ���   
>=0����ȡ��������Ŀ     
	*/   
	INT WMI_MacAddress( INT iQueryType, T_MACADDRESS *pMacAddress, INT iSize );    
    
#ifdef __cplusplus    
}    
 #endif   
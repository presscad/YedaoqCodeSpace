#pragma once

#include "Resource.h"

enum EnumCommandIDRange : WORD
{
	ToolBarCmbSearchID		= 0xF800,		// �������������ID
	ToolBarBtnFindNextID	= 0xF801,		// ��������ť��������һ������ID

	MinViewOpID				= 0xF830,		// ������ͼ���ṩ�ġ���������ID���䣬���ڴ���ÿ����ͼ�ϵ�˽�в�����ť��
	MaxViewOpID				= 0xFAFF,		
	
	MinMenuViewID			= 0xFB00,		// ������˵�������ͼ�������б��ж�̬�����Ĳ˵��ָ������ID���䡣
	MaxMenuViewID			= 0xFFFF,
};

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "GridCtrl_src\GridCellBase.h"
#include "GridCtrl_src\GridCell.h"
#include "GridCtrl_src\CellFactory.h"
#include "GridCtrl_src\GridCellComboBox.h"
#include "GridCtrl_src\GridCellCheck.h"
#include "GridCtrl_src\GridCellLink.h"
#include "GridCtrl_src\CellFactoryProvider.h"
#include "GridCtrl_src\CellTemplate.h"
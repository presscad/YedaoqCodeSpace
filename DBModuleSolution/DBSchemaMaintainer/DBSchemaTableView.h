
// DBSchemaTableView.h : CDBSchemaTableView ��Ľӿ�
//

#pragma once

#include "GridCtrl.h"
#include "Layout/FlowLayout.h"
#include "Layout/CtrlLayout.h"
#include "Module/Schema/DBTableSchema.h"
#include "Module/DBTable.h"
#include "View/DBColumnViewInfo.h"
#include "DBTableViewer4GridCtrl.h"

class CDBSchemaTableView : public CView
{
protected: // �������л�����
	CDBSchemaTableView();
	DECLARE_DYNCREATE(CDBSchemaTableView)

// ����
public:
	CDBSchemaMaintainerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// ʵ��
public:
	virtual ~CDBSchemaTableView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGridCtrl					GridCol;
	CGridCtrl					GridTab;
	NSYedaoqLayout::CFlowLayout Layouter;
	CComboBox					CmbTab;

	CDBTableViewColumnCollection GridTblColumns;
	CDBTableViewColumnCollection GridColColumns;

	CDBTableViewer4GridCtrl		GridColViewer;
	CDBTableViewer4GridCtrl		GridTabViewer;

	CDBColumnViewInfo			GridTab_Name;
	CDBColumnViewInfo			GridTab_DBName;
	CDBColumnViewInfo			GridTab_BuildIn;
	CDBColumnViewInfo			GridTab_DBExist;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DBSchemaTableView.cpp �еĵ��԰汾
inline CDBSchemaMaintainerDoc* CDBSchemaTableView::GetDocument() const
   { return reinterpret_cast<CDBSchemaMaintainerDoc*>(m_pDocument); }
#endif

class CDBTableInfo : public CDBRecordBase
{

};

class CDBTableInfoEnumerator : public IEnumerator<IDBRecord>
{
public:
	CDBTableInfoEnumerator(CDBModule* pModule)
		: DBModulePtr(pModule), InnerEnumPtr(pModule->Tables().Enum())
	{
		_ASSERT(pModule);
	}

	CDBTableInfoEnumerator(const CDBTableInfoEnumerator& other)
		: DBModulePtr(other.DBModulePtr), InnerEnumPtr(DBModulePtr->Tables().Enum())
	{}
	
	virtual bool MoveNext()
	{
		return InnerEnumPtr->MoveNext();
	}

	virtual bool MoveNext(IDBRecord& rec)
	{
		if(MoveNext())
		{
			rec = Current();
			return true;
		}

		return false;
	}

	virtual const IDBRecord& Current()
	{
		static CDBRecordAuto rec;

		CDBTableSchema& schema = InnerEnumPtr->Current()->GetSchema();
		rec.SetField(0, schema.Name);
		rec.SetField(1, schema.DBName);
		rec.SetField(2, schema.IsBuildin() ? TEXT("1") : TEXT("0"));
		rec.SetField(3, schema.IsDBExist() ? TEXT("1") : TEXT("0"));

		return rec;
	}

	virtual void Reset()
	{
		InnerEnumPtr = std::auto_ptr<DBTableEnumerator>(DBModulePtr->Tables().Enum());
	}

	virtual ICloneable* Clone() const
	{
		return new CDBTableInfoEnumerator(*this);
	}

protected:
	CDBModule*						 DBModulePtr;
	std::auto_ptr<DBTableEnumerator> InnerEnumPtr;
};

class CDBColumnInfoEnumerator : public IEnumerator<IDBRecord>
{
public:
};
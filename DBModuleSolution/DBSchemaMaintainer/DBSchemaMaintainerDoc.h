
// DBSchemaMaintainerDoc.h : CDBSchemaMaintainerDoc ��Ľӿ�
//

#pragma once
#include <set>
#include "CDBSchemaTableItem.h"
#include "Module\DBModule.h"

class CDBSchemaMaintainerDoc : public CDocument
{
protected: // �������л�����
	CDBSchemaMaintainerDoc();
	DECLARE_DYNCREATE(CDBSchemaMaintainerDoc)

// ����
public:
	virtual CDBModule& GetDBModule() { return DBModule_; }
	virtual CString& GetFile() { return File_; }

// ����
public:
	virtual int	LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping);
	virtual int LoadBuildInSchema(LPCTSTR lpszSchemaFile);
	virtual int SaveBuildInSchema();

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CDBSchemaMaintainerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	NSDBModule::CDBModule	DBModule_;
	CString					File_;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenDB();
};




// DBSchemaMaintainerDoc.h : CDBSchemaMaintainerDoc ��Ľӿ�
//

#pragma once
#include <set>
#include "CDBSchemaTableItem.h"
#include "Module\DBModule.h"
#include "DBInterface\DBSourceManager.h"
#include "DBInterface\DBNameMapping.h"
#include "DynamicLinkLibrary.h"

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
	int	LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping);
	int LoadBuildInSchema(LPCTSTR lpszSchemaFile);
	int SaveBuildInSchema();

	int	UpdateModuleAsBuildin();

	int MergeTable(int iTbl1, int iTbl2);
	int MergeColumn(int iTbl, int Col1, int Col2);

	bool FindDBProviderDLL(tstring& filePath);
	bool GetDBProvider(HMODULE hModule, IDBSourceManager** ppSourceManager, IDBNameMapping** ppNameMapping);

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
	NSDBModule::CDBModule			DBModule_;
	CString							File_;
	CDynamicLinkLibrary				DllDBProvider_;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenDB();
	afx_msg void OnOpenFile();
	afx_msg void OnFileSave();
};



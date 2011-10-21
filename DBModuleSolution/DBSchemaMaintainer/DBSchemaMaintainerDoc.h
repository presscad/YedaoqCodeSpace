
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
	virtual CDBModule&	GetDBModule() { return DBModule_; }
	virtual CString		GetBuildInSchemaDetailFile() { return ProjectPath_ + '\\' + ProjectName_ + '.' + ProjectExt_; }
	virtual CString		GetBuildInSchemaObjIDFile() { return ProjectPath_ + '\\' + ProjectName_ + TEXT("ObjEnum.h"); }

// ����
public:
	int	LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping);
	int LoadBuildInSchema(LPCTSTR lpszSchemaFile);
	int SaveBuildInSchemaDetail();
	int SaveBuildInSchemaObjID();

	int	UpdateModuleAsBuildin();

	int MergeTable(int iTbl1, int iTbl2);
	int MergeColumn(int iTbl, int Col1, int Col2);

	bool FindDBProviderDLL(tstring& filePath);
	bool GetDBProvider(HMODULE hModule, IDBSourceManager** ppSourceManager, IDBNameMapping** ppNameMapping);

	int ParseProjectInfo(LPCTSTR lpszProFile);

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
	CString							ProjectName_;
	CString							ProjectPath_;
	CString							ProjectExt_;
	CDynamicLinkLibrary				DllDBProvider_;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenDB();
	afx_msg void OnOpenFile();
	afx_msg void OnFileSave();
};



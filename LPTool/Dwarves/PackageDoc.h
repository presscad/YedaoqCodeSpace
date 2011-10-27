
// PackageDoc.h : CPackageDoc ��Ľӿ�
//


#pragma once
#include <Module\DBModule.h>


class CPackageDoc : public CDocument
{
public:
	CPackageDoc();

protected: // �������л�����
	
	DECLARE_DYNCREATE(CPackageDoc)

// ����
public:

// ����
public:

	virtual void SetDBModule(NSDBModule::CDBModule* pModule)
	{
		if(DBModule)
		{
			delete DBModule;
		}
		DBModule = pModule;
	}

	virtual NSDBModule::CDBModule* GetDBModule()
	{
		return DBModule;
	}

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CPackageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	NSDBModule::CDBModule* DBModule;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



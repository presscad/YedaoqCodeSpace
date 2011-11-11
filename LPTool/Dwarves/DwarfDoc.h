
// DwarfDoc.h : CDwarfDoc ��Ľӿ�
//


#pragma once
#include <Module\DBModule.h>


class CDwarfDoc : public CDocument
{
public:
	CDwarfDoc();

protected: // �������л�����
	
	DECLARE_DYNCREATE(CDwarfDoc)

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
	virtual ~CDwarfDoc();
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



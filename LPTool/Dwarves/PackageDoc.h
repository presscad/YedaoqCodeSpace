
// PackageDoc.h : CPackageDoc ��Ľӿ�
//


#pragma once


class CPackageDoc : public CDocument
{
protected: // �������л�����
	CPackageDoc();
	DECLARE_DYNCREATE(CPackageDoc)

// ����
public:

// ����
public:

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

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



#pragma once

#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"

template<>
class CDwarfViewInfoDBTable<TBL_FileInfo> : public CDwarfViewInfoDBTblBase
{
public:
	CDwarfViewInfoDBTable()
		: CDwarfViewInfoDBTblBase(&CDBModuleLP::GetInstance(), TBL_FileInfo)
	{}

	void UpdateFileInfo(const IDBRecord& recFile);

	virtual int InitializeOperations();

	afx_msg void OnFileinfoUpdate(DwarfViewOperationContext* pCtx); 
};

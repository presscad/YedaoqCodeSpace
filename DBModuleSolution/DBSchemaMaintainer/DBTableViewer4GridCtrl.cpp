#include "StdAfx.h"
#include "DBTableViewer4GridCtrl.h"
#include "GridCtrl.h"

using namespace NSDBModule;

int	CDBTableViewer4GridCtrl::Initialize(const CDBTableViewColumnCollection& columns)
{
	ColumnInfos_ = columns;

	if(HeadRowCount_ == -1) HeadRowCount_ = 1;
	Grid_->SetFixedRowCount(HeadRowCount_);
	Grid_->SetRowCount(HeadRowCount_);	

	std::auto_ptr<ColumnEnumerator> pEnumCol(ColumnInfos_.Enum());
	if(!pEnumCol.get())
	{
		_ASSERT(false);
		return -1;
	}
	
	DBTableViewColumn vCol;
	while(pEnumCol->MoveNext(vCol))
	{
		for(int i = Grid_->GetColumnCount() - 1; i < vCol.IdxView; ++i)
		{
			Grid_->InsertColumn(TEXT(""));
		}

		Grid_->DeleteColumn(vCol.IdxView);

		CGridColumn gridCol = GetGridColumnFromDBColInfo(vCol.ViewInfo);
		
		Grid_->InsertColumn(
			gridCol, 
			DT_CENTER|DT_VCENTER|DT_SINGLELINE, 
			vCol.IdxView);

		Grid_->SetItemText(0, vCol.IdxView, vCol.ViewInfo->GetTitle().c_str());
	}

	return 1;
}

CGridColumn	CDBTableViewer4GridCtrl::GetGridColumnFromDBColInfo(CDBColumnViewInfo* info)
{
	CCellTemplate tpl = CCellTemplate(info->GetTextFormat(), info->GetEditStyle());
	
	DWORD state = 0;
	if(info->GetReadOnly())
	{
		state |= GVIS_READONLY;
	}

	return CGridColumn(info->GetTitle(), info->GetDefaultWidth(), tpl, state);
}

int	CDBTableViewer4GridCtrl::Fill(IEnumerator<IDBRecord>& recs)
{
	while(recs.MoveNext())
	{
		NewRecordAt(-1, recs.Current());
	}

	return -1;
}

// record enumerate
int	CDBTableViewer4GridCtrl::GetRecordCount()
{
	return Grid_->GetRowCount() - HeadRowCount_;
}

IDBTableViewer::RecordEnumerator* CDBTableViewer4GridCtrl::EnumRecord()
{
	_ASSERT(false);
	return 0;
}

// selection info
int CDBTableViewer4GridCtrl::GetCurRecord(IDBRecord* rec)
{
	CCellRange range = Grid_->GetSelectedCellRange();
	if(!range.IsValid())
	{
		return GetRecordAt(range.GetMinRow() - HeadRowCount_, rec);
	}

	return -1;
}

DBTableViewColumn* CDBTableViewer4GridCtrl::GetCurColumn()
{
	DBTableViewColumn* pRet = 0;
	CCellRange range = Grid_->GetSelectedCellRange();
	if(!range.IsValid())
	{
		int iGridCol = range.GetMinCol();
		pRet = ColumnInfos_.GetColumnByViewCol(iGridCol);
	}

	return pRet;
}

// single record r/w
int CDBTableViewer4GridCtrl::GetRecordAt(int row, IDBRecord* rec)
{
	if(row < 0 || row >= GetRecordCount() || !rec)
	{
		_ASSERT(false);
		return -1;
	}
	else
	{
		row += HeadRowCount_;
	}
	
	std::auto_ptr<ColumnEnumerator> pEnumCol(ColumnInfos_.Enum());
	_ASSERT(pEnumCol.get());

	while(pEnumCol->MoveNext())
	{
		CGridCellBase* pCell = Grid_->GetCell(row, pEnumCol->Current().IdxView);
		rec->SetField(pEnumCol->Current().IdxRecord, pCell->GetValue());
	}

	return 1;
}

int CDBTableViewer4GridCtrl::DelRecordAt(int row, IDBRecord* rec)
{
	if(row < 0 || row >= GetRecordCount() || !rec)
	{
		_ASSERT(false);
		return -1;
	}
	else
	{
		GetRecordAt(row, rec);
		row += HeadRowCount_;
	}

	return Grid_->DeleteRow(row + HeadRowCount_) ? 1 : -1;
}

int CDBTableViewer4GridCtrl::SetRecordAt(int row, const IDBRecord& rec)
{
	if(row < 0 || row >= GetRecordCount())
	{
		_ASSERT(false);
		return -1;
	}
	else
	{
		row += HeadRowCount_;
	}

	std::auto_ptr<ColumnEnumerator> pEnumCol(ColumnInfos_.Enum());
	_ASSERT(pEnumCol.get());

	while(pEnumCol->MoveNext())
	{
		CGridCellBase* pCell = Grid_->GetCell(row, pEnumCol->Current().IdxView);
		pCell->SetValue(rec.GetField(pEnumCol->Current().IdxRecord).c_str());
	}

	return 1;
}

int CDBTableViewer4GridCtrl::NewRecordAt(int row, const IDBRecord& rec)
{
	if(row < -1)
	{
		_ASSERT(false);
		return -1;
	}
	else if(row > -1)
	{
		row += HeadRowCount_;
	}

	row = Grid_->InsertRow(TEXT(""), row);
	
	if(row >= 0)
	{
		return SetRecordAt(row - HeadRowCount_, rec);
	}
	else
	{
		_ASSERT(false);
	}

	return -1;
}

// column info
IEnumerator<DBTableViewColumn>* CDBTableViewer4GridCtrl::EnumColumn()
{
	return ColumnInfos_.Enum();
}
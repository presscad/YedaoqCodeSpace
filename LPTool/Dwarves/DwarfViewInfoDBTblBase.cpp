#include "StdAfx.h"
#include "DwarfViewInfoDBTblBase.h"
#include <Module\Schema\DBTableSchema.h>
#include <ITextFormater.h>
#include "DBTableViewer4GridCtrl.h"
#include <Module\EnumCppDataType.h>
#include <EditStyle.h>
#include "DBEnumeratorProvider.h"
#include <Util\DBEnumeratorSuit.h>
#include "DBMapFormatProvider.h"
#include "DBOptionalEditStyleProvider.h"
#include "DwarfViewProvider.h"

using namespace NSDBModule;

bool CDwarfViewInfoDBTblBase::IsDBTableRelated(int tblCur, int tblTar, int* colCur, int* colTar)
{
	if(tblCur < 0 || tblCur >= CDBModuleLP::GetInstance().Tables().Count() || tblTar < 0 || tblTar >= CDBModuleLP::GetInstance().Tables().Count())
	{
		return false;
	}

	std::auto_ptr<DBColumnEnumerator> pEnumCol(CDBModuleLP::GetInstance().Tables()[tblCur]->EnumColumn());
	while(pEnumCol->MoveNext())
	{
		if(pEnumCol->Current().RelyTblID == tblTar)
		{
			if(colCur) *colCur = pEnumCol->Current().Index;
			if(colTar) *colTar = pEnumCol->Current().RelyColID;
			return true;
		}
	}

	return false;
}

CDwarfViewInfoDBTblBase::~CDwarfViewInfoDBTblBase(void)
{
}

int CDwarfViewInfoDBTblBase::Initialize()
{
	if(Initialized()) return 1;

	InitializeViewColumns();
	InitializeOperations();
	InitializeReleatedViews();	

	return __super::Initialize();
}

int CDwarfViewInfoDBTblBase::InitializeReleatedViews()
{
	std::auto_ptr<DBTableEnumerator> pEnumTbl(this->DBModule->Tables().Enum());
	
	for(int idx = 0; idx < DBModule->Tables().Count(); ++idx)
	{
		if(IsDBTableRelated(idx, GetViewID(), 0, 0))
		{
			this->ReleatedViews.push_back(
				CDwarfViewProvider::GetInstance()[idx]);
		}
	}

	std::auto_ptr<DBColumnEnumerator> pEnumCol(this->DBModule->Tables()[this->GetViewID()]->EnumColumn());
	while(pEnumCol->MoveNext())
	{
		if(pEnumCol->Current().RelyTblID != DBColumnSchema::InvalidRelyTableID)
		{
			this->ReleatedViews.push_back(
				CDwarfViewProvider::GetInstance()[pEnumCol->Current().RelyTblID]);
		}
	}

	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeOperations()
{
	this->Operations.Append(TEXT("更新"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordModify));
	this->Operations.Append(TEXT("添加"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordInsert));
	this->Operations.Append(TEXT("删除"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTblBase::OnRecordDelete));
	
	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeViewColumns()
{
	InitializeColumnViewInfo();


	ViewColumns.AppendVirtual(&GetGridCol4Select(), 0);

	for (DBColumnViewInfoCollection::iterator iter = ColumnViewInfos.begin(); iter != ColumnViewInfos.end(); ++iter)
	{
		ViewColumns.Append(&(*iter));
	}
	return 1;
}

int CDwarfViewInfoDBTblBase::InitializeColumnViewInfo()
{
	CDBTableSchema& tblSchema = DBModule->Tables()[ViewID]->GetSchema();
	std::auto_ptr<DBColumnEnumerator> pEnumCol(tblSchema.EnumColumn());

	while(pEnumCol->MoveNext())
	{
		ColumnViewInfos.push_back(GenerateColumnViewFromSchema(pEnumCol->Current()));
	}
	return 1;
}

CDBColumnViewInfo CDwarfViewInfoDBTblBase::GenerateColumnViewFromSchema(const DBColumnSchema& col)
{
	CDBColumnViewInfo view;
	view.SetTitle(col.Name);
	view.SetDefaultWidth(-1);

	if(col.Type == EnumCppDataType::CppBool)
	{
		view.SetEditStyle(&CEditStyleBool::GetInstance());
		view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
	}
	else if(col.RelyTblID >= 0 && col.RelyColID >= 0)
	{
		if(col.VisiColID >= 0)
		{
			view.SetEditStyle(&CDBOptionalEditStyleProvider::GetInstance().Get(col.RelyTblID, col.VisiColID));
		}
		else
		{
			view.SetEditStyle(&CDBOptionalEditStyleProvider::GetInstance().Get(col.RelyTblID, col.RelyColID));
		}

		if(col.VisiColID >= 0 && col.VisiColID != col.RelyColID)
		{
			view.SetTextFormat(&CDBMapFormatProvider::GetInstance().Get(col.RelyTblID, col.RelyColID, col.VisiColID));
		}
		else
		{
			view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
		}
	}
	else
	{
		view.SetEditStyle(&CEditStyleNone::GetInstance());
		view.SetTextFormat(&CTextFormatSwitcherNone::GetInstance());
	}

	return view;
}

IEnumerator<IDBRecord>*	CDwarfViewInfoDBTblBase::EnumRecordAsRelatedView(DwarfViewOperationContext* pCtx)
{
	// 1. 判断从属视图是否依赖了主视图，若是，则根据该依赖关系过滤
	// 2. 判断主视图是否依赖了从属视图，若是，则根据该依赖关系过滤

	if(!pCtx)
	{
		_ASSERT(false);
		return 0;
	}

	int iRelatedTblID = DBColumnSchema::InvalidRelyTableID;
	int iRelatedColIDOfCurrentTbl = -1;
	int iRelatedColIDOfMainTbl = -1;

	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return 0;

	IDBRecord* pMainRec = pMainView->GetFocusedRecord();
	if(!pMainRec) return 0;

	if(IsDBTableRelated(GetViewID(), pMainView->GetViewID(), &iRelatedColIDOfCurrentTbl, &iRelatedColIDOfMainTbl))
	{
		iRelatedTblID = pMainView->GetViewID();
	}
	else if(IsDBTableRelated(pMainView->GetViewID(), GetViewID(), &iRelatedColIDOfMainTbl, &iRelatedColIDOfCurrentTbl))
	{
		iRelatedTblID = pMainView->GetViewID();
	}

	if(iRelatedTblID == DBColumnSchema::InvalidRelyTableID)
	{
		return 0;
	}

	CDBRecordBase rec = DBModule->Tables()[GetViewID()]->RecordTemplate();
	rec.SetField(iRelatedColIDOfCurrentTbl, pMainRec->GetField(iRelatedColIDOfMainTbl));
	CDBRecordComparison cmp(1, iRelatedColIDOfCurrentTbl);

	return DBModule->Tables()[GetViewID()]->FindAll(rec, cmp);
}

IEnumerator<IDBRecord>*	CDwarfViewInfoDBTblBase::EnumRecord()
{
	return DBModule->Tables()[GetViewID()]->EnumRecord();
}

void CDwarfViewInfoDBTblBase::OnRecordModify(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return;

	CDBRecordBase recOri = DBModule->Tables()[pMainView->GetViewID()]->RecordTemplate();
	CDBRecordBase recCur = recOri;

	if(pMainView->GetUpdatedRecord(&recCur, &recOri) <= 0)
	{
		return;
	}

	if(DBModule->Tables()[pMainView->GetViewID()]->Update(recCur, recOri) > 0)
	{
		pMainView->RemoveRecordUpdated();
		pMainView->AddRecord(recCur);
	}
	else
	{
		ASSERT(false);
	}
}

void CDwarfViewInfoDBTblBase::OnRecordDelete(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return;

	CDBRecordBase recOri = DBModule->Tables()[pMainView->GetViewID()]->RecordTemplate();

	if(pMainView->GetUpdatedRecord(0, &recOri) <= 0)
	{
		return;
	}

	if(DBModule->Tables()[pMainView->GetViewID()]->Delete(recOri) > 0)
	{
		pMainView->RemoveRecordUpdated();
	}
	else
	{
		ASSERT(false);
	}
}

void CDwarfViewInfoDBTblBase::OnRecordInsert(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return;

	CDBRecordBase recCur = DBModule->Tables()[pMainView->GetViewID()]->RecordTemplate();

	if(pMainView->GetUpdatedRecord(&recCur, 0) <= 0)
	{
		return;
	}

	if(DBModule->Tables()[pMainView->GetViewID()]->Insert(recCur) > 0)
	{
		pMainView->AddRecord(recCur);
	}
	else
	{
		ASSERT(false);
	}
}
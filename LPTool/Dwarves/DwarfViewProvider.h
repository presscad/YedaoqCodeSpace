#pragma once
#include <Singlton.h>
#include <map>
#include "DwarfViewInfo.h"
#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"
#include <Enumerator.h>

class CDwarfViewProvider : public CSingleton<CDwarfViewProvider>
{
public:
	typedef std::map<int, IDwarfViewInfo*> ViewMap;

	struct MapItem2DwarfViewInfo
	{
		IDwarfViewInfo* operator()(ViewMap::iterator iter){	return iter->second; }
	};

public:
	IDwarfViewInfo* operator[](int id)
	{
		ViewMap::iterator iter = Views.find(id);
		if(iter == Views.end())
		{
			_ASSERT(false);
			return 0;
		}

		return iter->second;
	}

	IEnumerator<IDwarfViewInfo*>* Enum()
	{
		return new_convert_enumerator<IDwarfViewInfo*>(
			CRangeEnumerator<ViewMap::iterator>(Views.begin(), Views.end()),
			MapItem2DwarfViewInfo());
	}

	CDwarfViewProvider()
	{
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CheckBytes>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CodecPin>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Codecs>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CodecType>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_FileInfo>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_FilterType>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_GuidInfo>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkInfo>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkInput>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkLink>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkRules>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Links>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_MediaExtension>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_MediaType>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageDepends>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageIndex>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageInfo>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageLua>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Parser>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_ParserCondition>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_ParserResult>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PlayerKernel>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Rules>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_SourceConfig>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_SourceLink>());
		Append(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Stream>());
	}

	void Append(IDwarfViewInfo* view)
	{
		Views.insert(std::make_pair(view->GetViewID(), view));
	}

protected:
	ViewMap Views;
};
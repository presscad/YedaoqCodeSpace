#pragma once

#include "DBInterface/DBDataAdapter.h"
#include "Module/DBRecord.h"
#include "EnumEntityHeaderMicro.h"
#include "Module/Schema/DBColumnSchema.h"
#include <boost/lexical_cast.hpp>

BEGINENUM(ModifyState, ״̬)
ENUMITEM0(Unknow)
ENUMITEM0(Uptodate)
ENUMITEM0(Add)
ENUMITEM0(Remove)
ENUMITEM0(Modify)
ENDENUM

BEGINENUM(DBSchemaItemCategory, ����)
ENUMITEM0(Table)
ENUMITEM0(Column)

ENDENUM

using namespace NSDBModule;

class CDBSchemaItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		default:
		case 3:
			_ASSERT(false);
			break;
		}
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		default:
			_ASSERT(false);
			break;
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 3; };

public:
	tstring						DBName;
	tstring						Name;
	tstring						Table;
	EnumModifyState				ModifyState;
	EnumDBSchemaItemCategory	Category;
	EnumCppDataType				CppType;
	IDBDataType*				DBType;
};

class CDBSchemaTableItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		default:
			_ASSERT(false);
			break;
		}
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		default:
			_ASSERT(false);
			break;
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 3; };

public:
	tstring			DBName;
	tstring			Name;
	EnumModifyState	ModifyState;
};

class CDBSchemaColumnItem : public IDBRecord
{
public:
	virtual const tstring& GetField(unsigned int i) const 
	{
		switch(i)
		{
		case 0:
			return DBName;
			break;
		case 1:
			return Name;
			break;
		case 2:
			return boost::lexical_cast<tstring>(ModifyState);
			break;
		case 3:
			return KeyColumn ? TEXT("1") : TEXT ("0");
		default:
			_ASSERT(false);
			break;
		}
		_ASSERT(false);
		return tstring();
	}

	virtual int	SetField(unsigned int i, const tstring& val)
	{
		switch(i)
		{
		case 0:
			DBName = val;
			break;
		case 1:
			Name = val;
			break;
		case 2:
			ModifyState = (EnumModifyState)boost::lexical_cast<int>(val.c_str());
			break;
		case 3:
			KeyColumn = ((val == TEXT("0")) ? false : true);
			break;
		default:
			_ASSERT(false);
		}
		return 1;
	}

	virtual index_t	GetFieldCount() const { return 4; };

public:
	tstring			DBName;
	tstring			Name;
	EnumModifyState	ModifyState;
	bool			KeyColumn;
};

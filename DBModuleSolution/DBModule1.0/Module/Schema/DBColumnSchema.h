#pragma once

#include "mytype.h"
#include <vector>
#include "..\CPPDataType.h"
#include "..\..\DBCommon\DBDataTypeUnknow.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface	IDBDataType;

	struct DBColumnSchema
	{
		typedef unsigned int flag_t;
		typedef unsigned int index_t;

		enum EnumFlag
		{
			None		= 0x0000,
			BuildIn		= 0x0001,	// indicate a column in buildin schema
			DBExist		= 0x0002,	// indicate a column exist in database
			KeyColumn	= 0x0004,	// indicate a column was a key field in object comparison

			DBPrimaryKey= 0x0010,	// indicate a column was a PK in database
			DBNullable	= 0x0020,	// indicate a column is nullable in database
		};

		index_t			Index;		// index of column in CDBRecordBase object
		tstring			Name;		// name of col in App
		EnumCppDataType Type;		// type of col in App(C++ type)

		index_t			DBIndex;	// index of column in database table schema
		tstring			DBName;		// name of column in database table schema
		IDBDataType*	DBType;		// type of column in database table schema

		flag_t			UniqueMask;	// every bit of value '1' of this member indicate that this column is unique in group of all other columns that with value '1' at the same bit
		flag_t			Flag;

		bool IsBuildin() const		{ return static_cast<bool>(Flag & BuildIn); }
		bool IsDBExist() const		{ return static_cast<bool>(Flag & DBExist); }
		bool IsKeyColumn() const	{ return static_cast<bool>(Flag & KeyColumn); }

		bool IsDBNullable() const	{ return static_cast<bool>(Flag & DBNullable); }
		bool IsDBPrimaryKey() const { return static_cast<bool>(Flag & DBPrimaryKey); }

		void SetFlag(flag_t flag, bool bAppend)
		{
			if(bAppend)
			{
				Flag |= flag;
			}
			else
			{
				Flag &= ~flag;
			}
		}

		void SetBuildInInfo(const tstring& name, EnumCppDataType type, bool bBuildIn, bool bKeyColumn)
		{
			Name = name;
			Type = type;
			SetFlag(BuildIn, bBuildIn);
			SetFlag(KeyColumn, bKeyColumn);
		}

		void SetExternInfo(const tstring& dbName, index_t dbIndex, IDBDataType* dbType, bool bExist, bool bPK, bool bNullable)
		{
			DBName = dbName;
			DBIndex = dbIndex;
			DBType = dbType;
			SetFlag(DBExist, bExist);
			SetFlag(DBPrimaryKey, bPK);
			SetFlag(DBNullable, bNullable);
		}

		void Reset()
		{
			ResetBuildinInfo();
			ResetExternInfo();
			Flag = 0;
			UniqueMask = 0;
		}
		void ResetBuildinInfo()
		{
			Index = -1;
			Name.clear();
			Type = CppUnknow;
			SetFlag(BuildIn, false);
			SetFlag(KeyColumn, false);
		}
		void ResetExternInfo()
		{
			DBIndex = -1;
			DBName.clear();
			DBType = &CDBDataTypeUnknow::GetInstance();
			SetFlag(DBExist, false);
			SetFlag(DBPrimaryKey, false);
			SetFlag(DBNullable, true);
		}
	};

	typedef std::vector<DBColumnSchema> DBColumnSchemaVct;
}
#pragma once

#include "DBDataType.h"

namespace NSDBModule
{
	class CDBDataTypeDecimalParser : public IDBDataTypeParser
	{
	public:
		virtual IDBDataType* Parse(const tstring& type);
	};

	class CDBDataTypeStringParser : public IDBDataTypeParser
	{
		virtual IDBDataType* Parse(const tstring& type);
	};
}
#include "DBDataTypeParser.h"
#include <regex>
#include "mytype.h"
#include "DBDataTypeCommon.h"
#include "boost/lexical_cast.hpp"

using namespace NSDBModule;

IDBDataType* CDBDataTypeDecimalParser::Parse(const tstring& type) const
{
	tregex reg(TEXT("^decimal(?:\\((\\d+),(\\d+)\\))?$"));
	tmatch match;
	if(std::tr1::regex_match(type, match, reg))
	{
		index_t length		= match[1].length() ? boost::lexical_cast<index_t>(match[0].str()) : -1;
		index_t precision	= match[2].length() ? boost::lexical_cast<index_t>(match[1].str()) : -1;

		return new CDBDataTypeDecimal(length, precision);
	}

	return 0;
}

IDBDataType* CDBDataTypeStringParser::Parse(const tstring& type) const
{
	tregex reg(TEXT("^((n)?(var)?char)\\((\\d+)\\)$"));
	tmatch match;
	if(std::tr1::regex_match(type, match, reg))
	{
		tstring name		= match[1].str();
		bool	bUnicode	= match[2].length() != 0;
		bool	bFixedWidth = match[3].length() == 0;
		index_t len			= boost::lexical_cast<index_t>(match[4].str());

		return new CDBDataTypeString(len, bFixedWidth, bUnicode);
	}

	return 0;
}
#include "stdafx.h"
#include "SqliteSource.h"
#include "SqliteDataAdapter.h"
#include "SqliteFactory.h"
#include "FileDialogST.h"
#include "SqliteNameMapping.h"

using namespace NSDBModule;

#define PathProperty TEXT("FilePath")

CSqliteConnection::CSqliteConnection()
	: Properties_(1)
{
	Properties_[0].Name = PathProperty;
}

CSqliteConnection::CSqliteConnection(const tstring& file)
	: Properties_(0)
{
	Properties_.push_back(DBConnectionProperty(PathProperty, file));
}

IDBConnection::PropertyEnumerator* CSqliteConnection::EnumProperty()
{
	return new_iterator_enumerator(Properties_.begin(), Properties_.end());
}

const tstring& CSqliteConnection::GetProperty(const tstring& name)
{
	if(name != PathProperty)
	{
		_ASSERT(false);
		throw std::exception();
	}

	return Properties_[0].Value;
}

bool CSqliteConnection::SetProperty(const tstring& name, const tstring& val)
{
	if(name != PathProperty)
	{
		_ASSERT(false);
		throw std::exception();
	}

	Properties_[0].Value = val;
	return true;
}

tstring	CSqliteConnection::ToString()
{
	return Properties_[0].Value;
}

const tstring& CSqliteSourceManager::ToString()
{
	static tstring Description(TEXT("Sqlite database engine"));
	return Description;
}

IDBConnection* CSqliteSourceManager::PromptSelectDBSource(IDBConnection* pDefault)
{	
	TCHAR pszFilter[] = TEXT("Sqlite Database File(*.db)|*.db|All Files(*.*)|*.*|");
	tstring fileName;

	if(pDefault)
	{
		fileName = pDefault->GetProperty(PathProperty);
	}

	CFileDialogST dlg(TRUE, NULL, fileName.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);

	//CFileDialog dlg(TRUE, NULL, fileName.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);

	if(IDOK != dlg.DoModal())
	{
		DWORD dwErr = GetLastError();
		return 0;
	}

	/*POSITION Pos = dlg.GetStartPosition();
	if (Pos == NULL)
	{
		return 0;
	}*/

	fileName = dlg.GetFileName();

	CSqliteConnection* pConn = new CSqliteConnection(fileName.c_str());
	return pConn;
}

IDBConnection* CSqliteSourceManager::PromptCreateDBSource(IDBConnection* pDefault)
{
	return 0;
}

IDBConnection* CSqliteSourceManager::ParseDBConnection(const tchar* pcConnStr)
{
	if(!pcConnStr)
	{
		_ASSERT(false);
		return 0;
	}

	if(0 == _taccess(pcConnStr, 0))
	{
		return new CSqliteConnection(pcConnStr);
	}
	else
	{
		return 0;
	}
}

bool CSqliteSourceManager::OpenDBConnection(IDBConnection* pConn, IDBDataAdapter** ppAdapter, IDBFactory** ppFactory)
{
	if(!pConn || !ppAdapter || !ppFactory)
	{
		_ASSERT(false);
		return false;
	}

	if(_taccess(pConn->ToString().c_str(), 0) != 0)
	{
		return false;
	}

	*ppAdapter = new CSqliteDataAdapter(pConn);
	*ppFactory = new CSqliteFactory();

	return true;
}

extern "C" __declspec(dllexport) IDBSourceManager* GetDBSourceManager()
{
	return new CSqliteSourceManager();
}

extern "C" __declspec(dllexport) IDBNameMapping* GetDBNameMapping()
{
	return new CSqliteNameMapping();
}
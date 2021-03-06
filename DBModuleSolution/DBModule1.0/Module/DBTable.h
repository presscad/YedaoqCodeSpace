#pragma once

#include "mytype.h"
#include <set>
#include <memory>
#include "Enumerator.h"
#include "DBRecord.h"
#include "Comaprison.h"
#include "DBRecordComparison.h"
#include ".\Schema\DBTableSchema.h"
#include ".\DMLNotifier\DMLNotifier.h"

namespace NSDBModule
{
	interface IDBRecord;
	interface IDBCommandBuilder;
	class CDBTableSchema;
	class CDBRecordComparison;
	class CDBRecordLessThan;
	class CDBModule;
	class CDBSchemaLoader;

	typedef IEnumerator<IDBRecord>								DBRecordEnumerator;
	typedef IEnumerator<DBColumnSchema>							DBColumnEnumerator;

	class CDBTable
	{
		typedef std::set<CDBRecordBase, CDBRecordLessThan>			DBRecordSet;
		typedef DBRecordSet::const_iterator							DBRecordIterator;
		typedef std::auto_ptr<IDBCommandBuilder>					DBCommandBuilderPtr;

		friend class CDBSchemaLoader;

	public:
		CDBTable(CDBModule* module, const CDBTableSchema& schema);
		CDBTable(CDBModule* module);
		CDBTable(const CDBTable& other);

		int InitializeStore();
		int initializeAccess();

		operator						const tstring&() const		{ return GetName(); }

		const tstring&					GetName() const			{ return Schema_.Name; }
		const tstring&					GetDBName() const		{ return Schema_.DBName; }
		const CDBRecordComparison&		GetComparison() const	{ return Comparison_; }
		const CDBTableSchema&			GetSchema()	const		{ return Schema_; }
		CDBTableSchema&					GetSchema()				{ return Schema_; }

		CDBModule*						GetDBModule() const		{ return DBModule_; }

		IEnumerator<DBColumnSchema>*	EnumColumn() const;
		IEnumerator<IDBRecord>*			EnumRecord();
		
		CDBRecordBase					RecordTemplate() const	{ return CDBRecordBase(GetSchema().Columns.size()); };

		int								LoadData();
		int								ClearData();

		bool							DataLoaded() const { return FlagLoaded_; }
		bool							DataDirty() const { return FlagDirty_; }
		bool							Invalidate() { FlagDirty_ = true; }

		int								Find(IDBRecord& rec);
		int								Find(int col, const tstring& val, IDBRecord& recDst);
		int								Find(IDBRecord& rec, const CDBRecordComparison& cmp);
		IEnumerator<IDBRecord>*			FindAll(const IDBRecord& rec, const CDBRecordComparison& cmp);
		IEnumerator<IDBRecord>*			FindAll(int col, const tstring& val);
	
		int								Update(const IDBRecord& cur, const IDBRecord& ori);
		int								Insert(const IDBRecord& rec);
		int								Delete(const IDBRecord& rec);

		bool							CallNotify(EnumDMLCommand cmd, const IDBRecord* cur, const IDBRecord* ori);

	protected:
		CDBTable& operator=(const CDBTable&);

		CDBModule*						DBModule_;
		CDBTableSchema					Schema_;
		CDBRecordComparison				Comparison_;
		CDBRecordLessThan				LessThan_;

		DBRecordSet						Records_;
		DBCommandBuilderPtr				CommandBuilder_;
		bool							FlagLoaded_;
		bool							FlagDirty_;
	};

}

/* ___________________________________________________________________________
@ 
@ file - DataTreeXmlStorage.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-5-23
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef DataTreeXmlStorage_h__
#define DataTreeXmlStorage_h__

#ifdef UNICODE
#define PUGIXML_WCHAR_MODE
#endif

#include "CPP/tstring.h"
#include "DataTreeCommon.h"
#include "pugixml/pugixml.hpp"
#include <fstream>

namespace nsYedaoqDataTree
{
	typedef pugi::xml_node		xnode_t;
	typedef pugi::xml_attribute	xattr_t;
	typedef pugi::xml_document	xdoc_t;

	class CDataTreeXmlStorage : public IDataTreeExporter, public IDataTreeImporter
	{
	public:
		CDataTreeXmlStorage(pugi::xml_node& doc, const tchar* nodename)
			: Root_(doc)
		{
			Current_ = GetChild(doc, nodename, true);
		}

		virtual tstring get_attr(const tchar* name, tstring def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.value();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, tstring const& val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val.c_str());
			return true;
		}

		virtual bool get_attr(const tchar* name, bool def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_bool();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, bool val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual double get_attr(const tchar* name, double def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_double();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, double val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual int get_attr(const tchar* name, int def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_int();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, int val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual unsigned int get_attr(const tchar* name, unsigned int def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_int();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, unsigned int val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual bool get_node(const tchar* name, IDataTreeNode& node)
		{
			pugi::xml_node xnode = GetChild(Current_, name, false);
			if(xnode)
			{
				Current_ = xnode;
				node.__import(*this);
				Current_ = Current_.parent();
			}

			return false;
		}

		virtual bool set_node(const tchar* name, IDataTreeNode const & node)
		{
			Current_ = GetChild(Current_, name, true);
			node.__export(*this);
			Current_ = Current_.parent();
			return true;
		}

		virtual bool get_nodes(const tchar* name, IDataTreeNodeInserter& nodes)
		{
			for(pugi::xml_node xnode = Current_.child(name); xnode; xnode = xnode.next_sibling(name))
			{
				Current_ = xnode;
				IDataTreeNode& node = nodes.InsertNew();
				node.__import(*this);
				Current_ = xnode.parent();
			}

			return true;
		}

		virtual bool set_nodes(const tchar* name, IEnumerator<IDataTreeNode>& nodes)
		{
			// 将数据输出到结点
			pugi::xml_node xnode = GetChild(Current_, name, true);
			pugi::xml_node tmp = Current_;
			while (xnode && nodes.MoveNext())
			{
				Current_ = xnode;
				nodes.Current().__export(*this);
				xnode = GetNeighbour(Current_, name, true);
			}
			Current_ = tmp;

			// 移除多余的同名结点
			while(tmp = xnode.next_sibling(name))
			{
				Current_.remove_child(xnode);
				xnode = tmp;
			}
			Current_.remove_child(xnode); // the last empty node

			return true;
		}

	protected:
		pugi::xml_node GetChild(pugi::xml_node& cur, const tchar* nodename, bool autoCreate = false)
		{
			pugi::xml_node node = cur.child(nodename);
			if(!node && autoCreate)
			{
				node = cur.append_child(nodename);
			}
			return node;
		}

		pugi::xml_node GetNeighbour(pugi::xml_node& cur, const tchar* nodename, bool autoCreate = false)
		{
			pugi::xml_node node = cur.next_sibling(nodename);
			if(!node && autoCreate)
			{
				node = cur.parent().append_child(nodename);
			}
			return node; 
		}

		CDataTreeXmlStorage& operator=(const CDataTreeXmlStorage&);

	protected:
		pugi::xml_node&		Root_;
		pugi::xml_node		Current_;
	};

	template<typename T>
	struct CXmlFile
	{
		CXmlFile(tchar const* file_path, tchar const* root_node)
			: file_path_(file_path), root_node_(root_node)
		{}

		bool Load(T&);
		bool Save(const T&);

	protected:
		tstring file_path_;
		tstring root_node_;
	};

	template<typename T>
	bool CXmlFile<T>::Save( const T& obj )
	{
		xdoc_t doc;
		nsYedaoqDataTree::CDataTreeXmlStorage storage(doc, root_node_.c_str());
		obj.__export(storage);

		std::locale prev_loc = std::locale::global(std::locale("chs"));
		tofstream f(file_path_, std::ios_base::out);
		doc.print(f);

		f.flush();
		f.close();

		return true;
	}

	template<typename T>
	bool CXmlFile<T>::Load( T& obj )
	{
		std::locale prev_loc = std::locale::global(std::locale("chs"));
		tifstream f(file_path_, std::ios_base::in);

		tstring content;
		std::getline(f, content, (tchar)EOF);

		tchar* bufXml = new tchar[content.size() + 1];
		content.copy(bufXml, content.size());
		bufXml[content.size()] = 0;

		xdoc_t doc;
		doc.load(content.c_str());
		nsYedaoqDataTree::CDataTreeXmlStorage storage(doc, root_node_.c_str());

		obj.__import(storage);

		return true;
	}
}

#endif // DataTreeXmlStorage_h__

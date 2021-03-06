/* ___________________________________________________________________________
@ 
@ file - C:\YedaoqCodeSpace\YedaoqCppLib\CPP\DataTree\DataTreeHeaderMicro.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-9-6
@ info -
@     
/* ___________________________________________________________________________*/
#ifndef DataTreeHeaderMicro_h__
#define DataTreeHeaderMicro_h__

#include "DataTreeCommon.h"

#ifndef BEGIN_DATATREE
	#define BEGIN_DATATREE(cppname) struct cppname : public nsYedaoqDataTree::IDataTreeNode { virtual bool __export(nsYedaoqDataTree::IDataTreeExporter& storage) const; virtual bool __import(nsYedaoqDataTree::IDataTreeImporter& storage);
#endif

#ifndef END_DATATREE
	#define END_DATATREE };
#endif

#ifndef ITEM_DATANODE
	#define ITEM_DATANODE(nodename, cppname, cpptype) cpptype cppname;
#endif

#ifndef ITEM_DATANODES
	#define ITEM_DATANODES(nodename, cppname, cpptype) std::vector<cpptype> cppname;
#endif

#ifndef ITEM_ATTR
	#define ITEM_ATTR(nodename, cppname, cpptype) cpptype cppname;
#endif

#ifndef ITEM_ENUMATTR
	#define ITEM_ENUMATTR(xmlname, cppname, cpptype) Enum##cpptype cppname;
#endif

#endif // DataTreeHeaderMicro_h__

#define _DATATREE_HELP_CHECK_UNDEFINE_HEADER_
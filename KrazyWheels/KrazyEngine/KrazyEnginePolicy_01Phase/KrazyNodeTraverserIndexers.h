#pragma once
#ifndef _KRAZYNODETRAVERSERINDEXERS_H_
#define _KRAZYNODETRAVERSERINDEXERS_H_
#include "KrazyPrimitiveDataCollections.h"
class KrazyXMLNode;
// need to have a generic cursor class ..,
struct _helper_indexer{
		_helper_indexer() :
		 _Cursor(0),
		 _Size(0)
		{}
		int					_Cursor;
		int					_Size;
		KrazyIntList		_List;
};
///////////////////////////////////////////////////////
class KrazyNodeDepthIndexerMap : public KrazyKeyPTRToObjectMap<KrazyXMLNode* , _helper_indexer>{};
//class KrazyIntToIndexer        : public hash_map<int , _helper_indexer*>{};
/////////////////////////////////////////////////////////
#endif
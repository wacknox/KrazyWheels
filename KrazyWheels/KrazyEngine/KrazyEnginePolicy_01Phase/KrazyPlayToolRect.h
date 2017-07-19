#pragma once
#include "KrazyWindow.h"
#define _KRAZY_PAUSE    string("    Pause")
#define _KRAZY_RESET    string("    Reset")
//...
class KrazyPlayToolRect
{
public:
	static KrazyPlayToolRect* get_default();
	static void release_default();
	//..,
	void reset();
	void onSetVectors(KrazyFloatList& _vectors);
	KrazyFloatList* get_vectors();
	///..,
    bool empty();
	//..
private:
	KrazyPlayToolRect();
	~KrazyPlayToolRect();
	//..,
	KrazyFloatList _Vectors;
	//..,
	static KrazyPlayToolRect* _Me;
};
//..,

#pragma once
#include "KrazyPrimitiveDataCollections.h"
//..,
enum KrazyColorName
{
	KRAZY_COLOR_Red     , 
	KRAZY_COLOR_Green   ,
	KRAZY_COLOR_Blue    , 
	KRAZY_COLOR_Yellow  ,
	KRAZY_COLOR_White   ,
	KRAZY_COLOR_Black   ,
	KRAZY_COLOR_Orange  ,
	KRAZY_COLOR_Purple  ,
	KRAZY_COLOR_Pink    ,
	KRAZY_COLOR_Brown   ,

};
//..,
static const float COLOR_GREY[]     = {0.3647f,0.3647f,0.3647f,1.0f};
static const float COLOR_WHITE[]    = {1.0f,1.0f,1.0f,1.0f};
static const float COLOR_RED[]      = {1.0f,0.0f,0.0f,1.0f};
static const float COLOR_BLUE[]     = {0.0f,0.0f,1.0f,1.0f};
static const float COLOR_GREEN[]    = {0.0f,1.0f,0.0f,1.0f};
static const float COLOR_YELLOW[]   = {1.0f,1.0f,0.0f,1.0f};
static const float COLOR_BLACK[]    = {0.0f,0.0f,0.0f,1.0f};
static const float COLOR_ORANGE[]   = {1.0f,0.5f,0.15f,1.0f};
static const float COLOR_PURPLE[]   = {0.26f,0.06f,0.33f,1.0f};
static const float COLOR_PINK[]     = {1.0f,0.5f,0.75f,1.0f};
static const float COLOR_BROWN[]    = {0.5f,0.25f,0.25f,1.0f};
static const float COLOR_ROAD_AMBIANCE[] = {0.5f ,  0.5f , 0.5f ,1.0f};
//..,
class KrazyFloatListColorDispensor
{
public:
	static KrazyFloatListColorDispensor* getDefault();
	static void releaseDefault();
private:
	KrazyFloatListColorDispensor()
	{
		on_setRed();
		on_setGreen();
		on_setBlue();
		on_setYellow();
		on_setWhite();
		on_setBlack();
		on_setOrange();
		on_setPurple();
		on_setPink();
		on_setBrown();
	}
public:
	KrazyFloatList& getColor(KrazyColorName _color)
	{
		//..,
		switch(_color)
		{
		case KRAZY_COLOR_Red     :   return _RedColor;
		case KRAZY_COLOR_Green   :   return _GreenColor;
		case KRAZY_COLOR_Blue    :   return _BlueColor; 
		case KRAZY_COLOR_Yellow  :   return _YellowColor;
		case KRAZY_COLOR_White   :   return _WhiteColor;
		case KRAZY_COLOR_Black   :   return _BlackColor;
		case KRAZY_COLOR_Orange  :   return _OrangeColor;
		case KRAZY_COLOR_Purple  :   return _PurpleColor;
		case KRAZY_COLOR_Pink    :   return _PinkColor;
		case KRAZY_COLOR_Brown   :   return _BrownColor;
		}
		//..,
		return _BlackColor;
	}

private:
	//..,
	void on_setRed()	{ _KRAZY_COPY_LIST_PTR(COLOR_RED     , _RedColor , 4);	    }
	void on_setGreen()	{ _KRAZY_COPY_LIST_PTR(COLOR_GREEN   , _GreenColor , 4);    }
	void on_setBlue()	{ _KRAZY_COPY_LIST_PTR(COLOR_BLUE    , _BlueColor , 4);     }
	void on_setYellow()	{ _KRAZY_COPY_LIST_PTR(COLOR_YELLOW  , _YellowColor , 4);   }
	void on_setWhite()	{ _KRAZY_COPY_LIST_PTR(COLOR_WHITE   , _WhiteColor , 4);    }
	void on_setBlack()	{ _KRAZY_COPY_LIST_PTR(COLOR_BLACK   , _BlackColor , 4);    }
	void on_setOrange()	{ _KRAZY_COPY_LIST_PTR(COLOR_ORANGE  , _OrangeColor , 4);   }
	void on_setPurple()	{ _KRAZY_COPY_LIST_PTR(COLOR_PURPLE  , _PurpleColor , 4);   }
	void on_setPink()	{ _KRAZY_COPY_LIST_PTR(COLOR_PINK    , _PinkColor , 4);     }
	void on_setBrown()	{ _KRAZY_COPY_LIST_PTR(COLOR_BROWN   , _BrownColor , 4);    }
	//..,
	KrazyFloatList _RedColor;
	KrazyFloatList _GreenColor;
	KrazyFloatList _BlueColor;
	KrazyFloatList _YellowColor;
	KrazyFloatList _WhiteColor;
	KrazyFloatList _BlackColor;
	KrazyFloatList _OrangeColor;
	KrazyFloatList _PurpleColor;
	KrazyFloatList _PinkColor;
	KrazyFloatList _BrownColor;
	static KrazyFloatListColorDispensor* _Me;
	//..,
};

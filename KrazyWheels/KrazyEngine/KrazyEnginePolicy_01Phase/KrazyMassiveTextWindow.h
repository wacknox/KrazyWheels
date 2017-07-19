#include "KrazyWindow.h"
#include "KrazyTextXML.h"
//..,
#define _KRAZY_SKIP_   string("Skip")
#define _KRAZY_NEXT_   string("Next")
#define _KRAZY_BACK_   string("Back")
//...,
class _KrazyList_to_KrazyTextList : public vector<KrazyTextList*>
{
public:
	~_KrazyList_to_KrazyTextList()
	{
		cleanUp();
	}
private:
	void cleanUp()
	{
		int _count = size();
		for(int i = 0; i < _count; i++){
			KrazyTextList* _text = at(i);
			delete _text;_text = 0;
		}
	}
};
//...,
class _KrazyString_to_KrazyText
{
public:
	_KrazyString_to_KrazyText(string _text);
	void convert();
	void convert_to_xml();
	KrazyTextList* _text_list();
	_KrazyList_to_KrazyTextList* _textarea_list();
	KrazyTextXML*  _xml();
private:
	string _format_space(string _text);
	KrazyFloatList& _color(int _color_index);
	//..,
	_KrazyList_to_KrazyTextList  _Krazylist_to_KrazyTextList;
	KrazyTextList _KrazyTextList;
	KrazyTextXML  _XML;
	string        _Text;
};
//
class KrazyMassiveTextArea
{
public :
	KrazyMassiveTextArea() :  
	_ShouldRelease(false)
    {
	}
    //..,
	~KrazyMassiveTextArea()
	{
		cleanUp();
	}
    ///..
	bool& should_release() { return _ShouldRelease; }
    ///..
    void cleanUp()
	{
		if(_ShouldRelease){
			int _count = _TextAreaList.size();
		    for(int i = 0; i < _count; i++){
				KrazyTextArea* _text_area = _TextAreaList[i];
			    delete _text_area;_text_area = 0;
			}
			_TextAreaList.clear();
		}	
    }
    //..,
	KrazyTextAreaList* get_textarealist() { return &_TextAreaList; }
    //..,
protected:
	KrazyTextAreaList _TextAreaList;
	bool			  _ShouldRelease;
};
//..,
class KrazyMassiveTextAreaList : public vector<KrazyMassiveTextArea*> {};
//..,
class KrazyStoryWindow : public KrazyWindow
{

public:
	KrazyStoryWindow();
	virtual ~KrazyStoryWindow();
	void cleanUp();
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	void append_text(KrazyStringList& text);
	void load_currentmassivetext();
	//..,
	void load_massivetext();
	void on_load();
	//..,
protected:
	KrazyMassiveTextAreaList    _MassiveTextList;
	unsigned int				_CurrentMassiveText;
	//..,
};
//..,
enum KrazyMovesWindowId
{
	KRAZY_MOVES_WINDOW_Id_a ,	
	KRAZY_MOVES_WINDOW_Id_b ,
};
//..,
class KrazyMovesWindow : public KrazyWindow
{

public:
	KrazyMovesWindow(KrazyMovesWindowId _window_Id);
	virtual ~KrazyMovesWindow();
	void cleanUp();
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	void append_text(KrazyStringList& text);
	void load_currentmassivetext();
	//..,
	void load_massivetext();
	void on_load();
#if 0
	virtual void render();
#endif
	virtual void on_touched(KrazyTextArea* _area);
	//..,
    void on_clean_a();
	void on_clean_b();
    //..,
protected:
	KrazyMassiveTextAreaList    _MassiveTextList;
	unsigned int				_CurrentMassiveText;
	KrazyMovesWindowId			_WindowId;
	//..,
};


#include "stdafx.h"
#include "KrazyMassiveTextWindow.h"
#include "KrazyColorDefinations.h"
//..,
#define _STORY_MAX_LINE		7
#define _MOVES_MAX_LINE     8

#define ATTR_NAME_DATA			string("data")
#define ATTR_NAME_BKCOLOR       string("bk_color")
#define ATTR_NAME_COLOR         string("color")
//..,
static float _krazy_massive_wnd[] = 
{
	0.95f ,  0.95f ,
   -0.95f ,  0.95f ,
    0.95f , -0.95f ,
	//..,
	-0.95f ,   0.95f ,
	-0.95f ,  -0.95f ,
	 0.95f , -0.95f 
};
//...,
static float _krazy_massive_client[] = 
{
	0.9f ,   0.9f ,
   -0.9f ,   0.9f ,
    0.9f ,  -0.9f ,
	//..,
	-0.9f ,   0.9f ,
    -0.9f ,  -0.9f ,
	 0.9f ,  -0.9f 
};
//..,
_KrazyString_to_KrazyText::_KrazyString_to_KrazyText(string _text) :
_Text(_text)
{
}
//..,
KrazyFloatList& _KrazyString_to_KrazyText::_color(int _color_index)
{
//....,,
	switch(_color_index)
	{
	case 0  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	case 1  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	case 2  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	case 3  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	case 4  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	case 7  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	case 8  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Orange);
	case 9  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Purple);
	case 10  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Pink);
	case 11  : return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Brown);
	}
//....,,
	return KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
}
//..,
KrazyTextXML*  _KrazyString_to_KrazyText::_xml()
{
	
	return &_XML;
}
//..,
void _KrazyString_to_KrazyText::convert_to_xml()
{
	_XML.loadText(_Text);
	int _ta_count = _XML.get_Kids_Count();
	for(int _ta = 0; _ta <  _ta_count; _ta++){
		KrazyXMLNode* _ta_node = _XML.get_KidsNode(_ta);
		int _kt_count = _ta_node->get_Kids_Count();
		KrazyTextList* _kt_list = new KrazyTextList;
		for(int _kt = 0; _kt < _kt_count; _kt++){
			KrazyXMLNode* _kt_node = _ta_node->get_KidsNode(_kt);
			KrazyXMLAttribute* _data = _kt_node->get_AttributeByTypeIfAny(ATTR_NAME_DATA);
			KrazyXMLAttribute*_a_color = _kt_node->get_AttributeByTypeIfAny(ATTR_NAME_COLOR);
			KrazyXMLAttribute* _bk_color = _kt_node->get_AttributeByTypeIfAny(ATTR_NAME_BKCOLOR);
			//..,
			string _text = _format_space(_data->get_Data());
			int _i_color;_KRAZY_GET_INT(_i_color,_a_color);
			int _i_bkcolor;_KRAZY_GET_INT(_i_bkcolor,_bk_color);
			//..,
			KrazyText* _kt_text = new KrazyText;
			_kt_text->on_setText(_text);
			_kt_text->on_setColor(_color(_i_color));
			_kt_text->on_setBkColor(_color(_i_bkcolor));
			_kt_list->push_back(_kt_text);
		}
		_Krazylist_to_KrazyTextList.push_back(_kt_list);
	}
}
//..,
void _KrazyString_to_KrazyText::convert()
{
	int _len = _Text.size();int _index = 0;
	int _count_krazy_text = _Text[_index++] - '0';
	for(int _thrive = 0;_thrive < _count_krazy_text;_thrive++){
		KrazyText* _text = new KrazyText;
		int _color_ = _Text[_index++] - '0';_text->on_setColor(_color(_color_));
		 _color_    = _Text[_index++] - '0';_text->on_setBkColor(_color(_color_));
		int _start = _index;_index = _Text.find('+' , _start);
		int _e_index = _index - _start;_index++;
		_text->on_setText(_Text.substr(_start , _e_index));
		_KrazyTextList.push_back(_text);
	}
}
//..,
string _KrazyString_to_KrazyText::_format_space(string _text)
{
	string _formatted;
	string _t_find  = "++";
	int _find_pos = 0;
	for(;;){
		int _index = _text.find(_t_find , _find_pos);
		if(_index == -1)
			break;
		
		_text.replace(_index , 2 , " ");
		_find_pos = _index;
	}
	//..,
	_formatted = _text;
	return _formatted;
}
//..,
KrazyTextList* _KrazyString_to_KrazyText::_text_list()
{
	return &_KrazyTextList;
}
//..,
_KrazyList_to_KrazyTextList* _KrazyString_to_KrazyText::_textarea_list()
{
	return &_Krazylist_to_KrazyTextList;
}
//..,
KrazyStoryWindow::KrazyStoryWindow() : 
_CurrentMassiveText(0)
{
	load_massivetext();
}
//..,
KrazyStoryWindow::~KrazyStoryWindow()
{
	cleanUp();
}
//..,
void KrazyStoryWindow::cleanUp()
{
	int _count = _MassiveTextList.size();
	for(int i =  0; i < _count; i++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[0];
		delete _massive_text;_massive_text = 0;
	}
	_MassiveTextList.clear();
}
//..,
void KrazyStoryWindow::load_massivetext()
{
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//............................................................................................................
	KrazyMassiveTextArea* _massive_text = new KrazyMassiveTextArea;
	_MassiveTextList.push_back(_massive_text);
	KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
	//..
	KrazyFloatList _rect;
	KrazyTextArea*  _textarea = 0;
	float _y = 0.2f;
	for(int i = 0; i < _STORY_MAX_LINE; i++){
		_rect.clear();_rect.push_back(_krazy_massive_client[2] + 0.05f);
		_rect.push_back(_krazy_massive_client[3] - _y);_rect.push_back(_krazy_massive_client[0]);
		_textarea = new KrazyTextArea();
		_textarea->on_setRect(_rect);
		_textarea->on_setBackGroundColor(_black);
		_textarea->on_setForeGroundColor(_green);
		_massive_area_text_list->push_back(_textarea);
		_y += 0.2f;
	}
	//..,
	_rect.clear();_rect.push_back(_krazy_massive_client[2] + 0.75f);
	_rect.push_back(_krazy_massive_client[3] - (_y + 0.2f));_rect.push_back(_krazy_massive_client[0]);
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_yellow);
	_massive_area_text_list->push_back(_textarea);
}
//..,
void KrazyStoryWindow::append_text(KrazyStringList& _text_list)
{
#if 1
	int _text_index = 0;
	_KrazyString_to_KrazyText _convert = _text_list[_text_index++];
	_convert.convert_to_xml();
	_KrazyList_to_KrazyTextList* _ta_list = _convert._textarea_list();
    //..,
	int _massive_count = _MassiveTextList.size();
	for(int _t = 0; _t < _massive_count; _t++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[_t];
		KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
		int _count = _massive_area_text_list->size();
		int i = 0;
		for(;i < _STORY_MAX_LINE ; i++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i);
			KrazyTextList* _kt_list = _ta_list->at(i);
			int _kt_count = _kt_list->size();
			for(int _kt = 0; _kt < _kt_count; _kt++){
				KrazyText* _kt_ = _kt_list->at(_kt);
				_text_area->on_append_text(_kt_);
			}
		    _text_area->calculate_textlist();
		}
		//..,
		for(int j = 0; j < 1; j++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i++);
			_text_area->on_setText(_text_list[_text_index++]);
			_text_area->calculate_textarea();
		}
	}
#else
	KrazyMassiveTextArea* _massive_text = _MassiveTextList[_CurrentMassiveText];
	KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
	int _count = _massive_area_text_list->size();
	for(int i = 0; i < _count; i++){
		KrazyTextArea* _text_area = _massive_area_text_list->at(i);
		_text_area->on_setText(_text_list[i]);
		_text_area->calculate_textarea();
	}
#endif
}
//..,
void KrazyStoryWindow::create(KrazyWindowStyle _ws_/* = KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//
void KrazyStoryWindow::load_currentmassivetext()
{
	_TextAreaList.clear();
	KrazyMassiveTextArea* _massive_text = _MassiveTextList[_CurrentMassiveText];
	KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
	int _count = _massive_area_text_list->size();
	for(int i = 0; i < _count; i++){
		_TextAreaList.push_back(_massive_area_text_list->at(i));
	}
}
//
void KrazyStoryWindow::on_load()
{
	on_setWndRect(_krazy_massive_wnd);
	on_setClientRect(_krazy_massive_client);
	load_currentmassivetext();
}
//..,
KrazyMovesWindow::KrazyMovesWindow(KrazyMovesWindowId _window_Id) : 
  _CurrentMassiveText(0)
, _WindowId(_window_Id)
{
	load_massivetext();
}
//..,
KrazyMovesWindow::~KrazyMovesWindow()
{
	cleanUp();
}
//..,
void KrazyMovesWindow::cleanUp()
{
	int _count = _MassiveTextList.size();
	for(int i =  0; i < _count; i++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[i];
		delete _massive_text;_massive_text = 0;
	}
	//..,
	_MassiveTextList.clear();
	//_TextAreaList.clear();
}
//..,
void KrazyMovesWindow::load_massivetext()
{
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//............................................................................................................
	float _touch_y = 0.0f;
	int _max_line = 0;
	switch(_WindowId)
	{
	case KRAZY_MOVES_WINDOW_Id_a	:	_max_line = 8; break;
	case KRAZY_MOVES_WINDOW_Id_b	:	_max_line = 7; break;
	}
	//..,
	KrazyMassiveTextArea* _massive_text = new KrazyMassiveTextArea;
	//_massive_text->should_release() = true;
	_MassiveTextList.push_back(_massive_text);
	KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
	//..
	KrazyFloatList _rect;
	KrazyTextArea*  _textarea = 0;
	float _y = 0.2f;
	for(int i = 0; i < _max_line; i++){
		_rect.clear();_rect.push_back(_krazy_massive_client[2] + 0.05f);
		_rect.push_back(_krazy_massive_client[3] - _y);_rect.push_back(_krazy_massive_client[0]);
		_textarea = new KrazyTextArea();
		_textarea->on_setRect(_rect);
		_textarea->on_setBackGroundColor(_black);
		_textarea->on_setForeGroundColor(_green);
		_massive_area_text_list->push_back(_textarea);
		_y += 0.2f;
	}
	//..,
	if(!(_touch_y > 0.0f)){
		_touch_y = _y + 0.003f;
	}
	//..,
	float _bottom = (_krazy_massive_client[3] - (_touch_y));
	float _left   = (_krazy_massive_client[2] + 0.75f) ;
	float _top    = (_bottom + 0.136125684f);  
	float _right  = (_left + 0.0550000072f);
	//..,
	KrazyFloatList _touch_rect;
	_touch_rect.push_back(_left);  _touch_rect.push_back(_top);
	_touch_rect.push_back(_right); _touch_rect.push_back(_bottom);
	//..,
	_rect.clear();_rect.push_back(_krazy_massive_client[2] + 0.75f);
	_rect.push_back(_krazy_massive_client[3] - _touch_y);_rect.push_back(_krazy_massive_client[0]);
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_setRect(_rect);
	_textarea->on_setTouchRect(_touch_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_yellow);
	_massive_area_text_list->push_back(_textarea);
	//..,
	KrazyFloatList* _rext_t = _textarea->get_Rect();
	_rect.clear();_rect.push_back(_rext_t->at(0) + 0.3f);
	_rect.push_back(_krazy_massive_client[3] - _touch_y);_rect.push_back(_krazy_massive_client[0]);
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_yellow);
	_massive_area_text_list->push_back(_textarea);

}
//..,
#if 1
void KrazyMovesWindow::append_text(KrazyStringList& _text_list)
{
#if 0
	int _massive_count = _MassiveTextList.size();
	for(int _t = 0; _t < _massive_count; _t++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[_t];
		KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
		int _count = _massive_area_text_list->size();
		int i = 0;
		for(;i < _count - 2 ; i++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i);
			_KrazyString_to_KrazyText _convert = _text_list[i];
			_convert.convert();
			KrazyTextList* _k_list = _convert._text_list();
			int _k_count = _k_list->size();
			for(int _k = 0; _k < _k_count; _k++){
				KrazyText* _text = _k_list->at(_k);
				_text_area->on_append_text(_text);
			}
		    _text_area->calculate_textlist();
		}
		//..,
		for(int j = i; j < _count; j++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(j);
			_text_area->on_setText(_text_list[j]);
			_text_area->calculate_textarea();
		}
	}
#else
	int _text_index = 0;
	_KrazyString_to_KrazyText _convert = _text_list[_text_index++];
	_convert.convert_to_xml();
	_KrazyList_to_KrazyTextList* _ta_list = _convert._textarea_list();
    //..,
	int _massive_count = _MassiveTextList.size();
	for(int _t = 0; _t < _massive_count; _t++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[_t];
		KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
		int _count = _massive_area_text_list->size();
		int i = 0;
		for(;i < _count - 2 ; i++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i);
			KrazyTextList* _kt_list = _ta_list->at(i);
			int _kt_count = _kt_list->size();
			for(int _kt = 0; _kt < _kt_count; _kt++){
				KrazyText* _kt_ = _kt_list->at(_kt);
				_text_area->on_append_text(_kt_);
			}
		    _text_area->calculate_textlist();
		}
		//..,
		for(int j = 0; j < 2; j++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i++);
			_text_area->on_setText(_text_list[_text_index++]);
			_text_area->calculate_textarea();
		}
	}
#endif
}
#else
void KrazyMovesWindow::append_text(KrazyStringList& _text_list)
{
	int _massive_count = _MassiveTextList.size();
	for(int _t = 0; _t < _massive_count; _t++){
		KrazyMassiveTextArea* _massive_text = _MassiveTextList[_t];
		KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
		int _count = _massive_area_text_list->size();
		for(int i = 0; i < _count; i++){
			KrazyTextArea* _text_area = _massive_area_text_list->at(i);
			_text_area->on_setText(_text_list[i]);
			_text_area->calculate_textarea();
		}
	}
}
#endif
//..,
void KrazyMovesWindow::create(KrazyWindowStyle _ws_/* = KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//
void KrazyMovesWindow::load_currentmassivetext()
{
	_TextAreaList.clear();
	KrazyMassiveTextArea* _massive_text = _MassiveTextList[_CurrentMassiveText];
	KrazyTextAreaList* _massive_area_text_list = _massive_text->get_textarealist();
	int _count = _massive_area_text_list->size();
	for(int i = 0; i < _count; i++){
		_TextAreaList.push_back(_massive_area_text_list->at(i));
	}
}
//
void KrazyMovesWindow::on_load()
{
	on_setWndRect(_krazy_massive_wnd);
	on_setClientRect(_krazy_massive_client);
	load_currentmassivetext();
}
//..,
#if 0
void KrazyMovesWindow::render()
{
	switch(_WindowState)
	{
	case KRAZY_MOVES_WINDOW_STATE_Render  : KrazyWindow::render(); break;
	case KRAZY_MOVES_WINDOW_STATE_Clean_a : on_clean_a();		   break;
	case KRAZY_MOVES_WINDOW_STATE_Clean_b : on_clean_b();		   break;
	}
}
#endif
//..,
void KrazyMovesWindow::on_touched(KrazyTextArea* _area)
{
#if 0
	string _text = _area->get_Text();
	if( _text == _KRAZY_NEXT_ ){
		_WindowState = KRAZY_MOVES_WINDOW_STATE_Clean_a;
	}
	else if(_text == _KRAZY_BACK_){
		_WindowState = KRAZY_MOVES_WINDOW_STATE_Clean_b;
	}
#endif
}
//..,
void KrazyMovesWindow::on_clean_a()
{
#if 0
	_CurrentMassiveText = 1;
	load_currentmassivetext();
	_WindowState = KRAZY_MOVES_WINDOW_STATE_Render;
#endif
}
//..,
void KrazyMovesWindow::on_clean_b()
{
#if 0
	_CurrentMassiveText = 0;
	load_currentmassivetext();
	_WindowState = KRAZY_MOVES_WINDOW_STATE_Render;
#endif
}
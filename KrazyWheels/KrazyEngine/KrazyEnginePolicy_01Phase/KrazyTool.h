#pragma once
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyWindow.h"
#include "KrazyProgramme.h"
//..,
enum KrazyToolButtonId
{
	KRAZY_TOOL_BUTTON_Id_Pause , 
	KRAZY_TOOL_BUTTON_Id_Reset , 
};
//.
class KrazyToolButton
{
public:
	KrazyToolButton(KrazyToolButtonId _tool_button_id);
	virtual ~KrazyToolButton(void);
	void reset();
	KrazyBool on_touch(float _x , float _y);
	KrazyToolButtonId get_id();
	//..,
	void on_set_background_color(KrazyFloatList& _color);
	void on_set_background_rect(KrazyFloatList& _rect);
	void on_set_tool_color(KrazyFloatList& _color);
	void on_set_tool_rect(KrazyFloatList& _rect);
	void on_set_scale(KrazyFloatList& _scale);
	void on_set_vector(glm::vec2& _vec);
	//..,
	KrazyFloatList* get_background_color();
	KrazyFloatList* get_background_rect();
	KrazyFloatList* get_tool_color();
	KrazyFloatList* get_tool_rect();
	KrazyFloatList* get_scale();
	glm::vec2*		get_vector();
	//..,
private:
	KrazyFloatList	_BackgroundColor;
	KrazyFloatList	_BackgroundRect;
	KrazyFloatList  _ToolColor;
	KrazyFloatList  _ToolRect;
	KrazyFloatList  _Scale;
	glm::vec2		_Vector;
	KrazyToolButtonId _Id;

};
//..,
class KrazyToolButtonList : public vector<KrazyToolButton*>{};
//..,
class KrazyToolRendrer
{
private:
	KrazyToolRendrer();
	~KrazyToolRendrer();
public:
	static KrazyToolRendrer* getDefault();
	static void releaseDefault();
public:
	void reset();
	void create();
    void render(KrazyToolButton* _tool_button);
//..,
protected:
	KrazyProgramme _Programme;
	unsigned int _Projection;
	unsigned int _WindowScale;
	unsigned int _ModelView;
	unsigned int _Position;
	unsigned int _Color;
	static KrazyToolRendrer* _Me;
};
//..,
class KrazyWheelsRendrer;
class KrazyToolWindow : public KrazyWindow
{
public:
	KrazyToolWindow(KrazyWheelsRendrer* _rendrer);
	virtual ~KrazyToolWindow();
	void reset();
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	virtual void render();
	virtual KrazyBool on_touch(float _x , float _y);
	virtual void on_refresh();
	//..,
	void on_load();
protected:
	void cleanUp();
	KrazyToolButtonList  _ToolButtonList;
	KrazyToolRendrer*    _ToolRendrer;
	KrazyWheelsRendrer*  _Rendrer;
};
//..,



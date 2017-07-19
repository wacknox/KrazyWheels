#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\mat4x4.hpp"
#include "glm\mat3x3.hpp"
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "KrazyProgramme.h"
#include <vector>
using namespace std;
class KrazyFloatList : public vector<float>{};
class KrazyDoubleLaneVertices
{
public:
	void onSetVertices(KrazyFloatList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++){
			_Vertices.push_back(list[i]);
		}
	}
	//
	void onSetBorders(KrazyFloatList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++){
			_Borders.push_back(list[i]);
		}
	}
	void copy(KrazyDoubleLaneVertices& _verts)
	{
		onSetVertices(*_verts.getVertices());
		onSetBorders(*_verts.getBorders());
	}
	//
	KrazyFloatList* getVertices(){ return &_Vertices; }
	KrazyFloatList* getBorders(){ return &_Borders; }
	//
private:
	KrazyFloatList _Vertices;
	KrazyFloatList _Borders;
};
//
class KrazyShadedDoubleLaneRendrer
{
public:
	KrazyShadedDoubleLaneRendrer(void);
	virtual ~KrazyShadedDoubleLaneRendrer(void);

	void create();
	void on_resize(int width , int height);
	void render();
	void on_vertices(KrazyDoubleLaneVertices& vert_lanes , KrazyDoubleLaneVertices& horz_lanes);

private:
	unsigned int _Projection;
	unsigned int _WindowScale;
	unsigned int _ModelView;
	unsigned int _Color;
	unsigned int _Position;

	glm::mat4 _ProjectionMatrix;
	glm::mat4 _WindowScaleMatrix;
	KrazyProgramme _Programme;
	KrazyDoubleLaneVertices _VerticalLanes;
	KrazyDoubleLaneVertices _HorizontalLanes;

	float _Width;
	float _Height;

};


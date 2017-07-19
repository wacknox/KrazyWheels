#include "stdafx.h"
#include "KrazyShadedDoubleLaneRendrer.h"
#include "KrazyShader.h"
#include "GLeish.h"
//
KrazyShadedDoubleLaneRendrer::KrazyShadedDoubleLaneRendrer(void)
{
}
//
KrazyShadedDoubleLaneRendrer::~KrazyShadedDoubleLaneRendrer(void)
{
}
//
void KrazyShadedDoubleLaneRendrer::create()
{
	char _vertex_shader[] = 
    "attribute vec3 inputPosition;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 window_scale;\n"
	"uniform mat4 modelview;\n"
	"void main(){\n"
	  "gl_Position  = projection * window_scale * modelview * vec4(inputPosition , 1.0);\n"
	 "}\n";

	char _fragment_shader[] = 
		"uniform vec4 color;\n"
		"void main(){\n"
		"gl_FragColor = color;\n"
		"}\n";

	KrazyShader vertexShader;vertexShader.LoadShader(GL_VERTEX_SHADER,_vertex_shader);
    KrazyShader fragmentShader;fragmentShader.LoadShader(GL_FRAGMENT_SHADER,_fragment_shader);
	_Programme.create(vertexShader,fragmentShader);_Programme.compile();
	//
	_Position = glGetAttribLocation(_Programme, "inputPosition");
	_Projection  = glGetUniformLocation(_Programme, "projection");
	_WindowScale = glGetUniformLocation(_Programme, "window_scale");
	_ModelView = glGetUniformLocation(_Programme, "modelview");
	_Color = glGetUniformLocation(_Programme, "color");
}
//
void KrazyShadedDoubleLaneRendrer::on_resize(int width , int height)
{
	_Width = float(width);_Height = float(height);
	float aspect = _Width/_Height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near   = -1.0f;
	float far    =  1.0f;

	_WindowScaleMatrix = glm::mat4((aspect - 0.015f), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 0.98f , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	_ProjectionMatrix = glm::ortho(left , right , bottom , top , near , far);
}
//
const float DEG_2_RAD_RR = 3.141593f / 180.0f;
void KrazyShadedDoubleLaneRendrer::render()
{
	glUseProgram(_Programme);
	//rendering vertical lanes..,
	KrazyDoubleLaneVertices* _vertices_lane = 0;
	for(int i = 0; i < 2; i++){
		switch(i){
		case 0:_vertices_lane = &_VerticalLanes;break;
		case 1:_vertices_lane = &_HorizontalLanes;break;
		}

		float _x = 0.0f;
		float _y = 0.0f;

		glm::mat4 view(1.0f , 0.0f , 0.0f ,0.0f,
			0.0f , 1.0f , 0.0f , 0.0f,
			0.0f , 0.0f , 1.0f , 0.0f,
			_x , _y , 0.0f , 1.0f);

		float angle = 0.0f;
		angle *= DEG_2_RAD_RR;

		glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
			-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
			0.0f , 0.0f , 1.0f , 0.0f,
			0.0f , 0.0f , 0.0f , 1.0f);

		glm::mat4 scale(1.0f , 0.0f , 0.0f , 0.0f,
			0.0f , 1.0f , 0.0f , 0.0f,
			0.0f , 0.0f , 1.0f , 0.0f,
			0.0f , 0.0f , 0.0f , 1.0f);

		glm::mat4 modelview = view * model * scale;

		glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_ProjectionMatrix));
		glUniformMatrix4fv(_WindowScale, 1, false, glm::value_ptr(_WindowScaleMatrix));
		glUniformMatrix4fv(_ModelView, 1, false, glm::value_ptr(modelview));
		float road_color[] = {0.2f , 0.2f , 0.2f , 1.0f};
		glUniform4fv(_Color, 1 , road_color);
		KrazyFloatList* vertices = _vertices_lane->getVertices();
		glVertexAttribPointer(_Position, 3, GL_FLOAT, GL_FALSE, 0, &vertices->at(0));
		glEnableVertexAttribArray(_Position);
		glDrawArrays(GL_TRIANGLES , 0 , vertices->size()/3);
		glDisableVertexAttribArray(_Position);
		//borders..,
#if 1
		KrazyFloatList* borders = _vertices_lane->getBorders();
		float border_color[] = {1.0f , 1.0f , 0.0f , 1.0f};
		glUniform4fv(_Color, 1 , border_color);
		glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &borders->at(0));
		glEnableVertexAttribArray(_Position);
		glDrawArrays(GL_LINES , 0 , borders->size()/2);
		glDisableVertexAttribArray(_Position);
#endif	
	}
}
//
void KrazyShadedDoubleLaneRendrer::on_vertices(KrazyDoubleLaneVertices& vert_lanes , KrazyDoubleLaneVertices& horz_lanes)
{
	_VerticalLanes.copy(vert_lanes);
	_HorizontalLanes.copy(horz_lanes);
}
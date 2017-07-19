#include "stdafx.h"
#include "KrazyShadedVehicalRendrer.h"
#include "KrazyShader.h"
#include "GLeish.h"
#define MAX_WAIT  40
//
KrazyShadedVehicalRendrer::KrazyShadedVehicalRendrer(void) :
_Width(0)
,_Height(0)
{
	_VehicalScaleMatrix = glm::mat4(1.0f , 0.0f , 0.0f , 0.0f,
							 1.0f , 1.0f , 0.0f , 0.0f,
							 1.0f , 0.0f , 1.0f , 0.0f,
							 1.0f , 0.0f , 0.0f , 1.0f);
}
//
KrazyShadedVehicalRendrer::~KrazyShadedVehicalRendrer(void)
{
}
//
void KrazyShadedVehicalRendrer::reset()
{
	_ProgrammeVehicalBody.reset();
	_PositionVehicalBody = 0;		
	_NormalVehicalBody	 = 0;		
	_ProjectVehicalBody	 = 0;		
	_WindowScaleVehicalBody = 0;		
	_ModelViewVehicalBody	= 0;	
	_ModelFragVehicalBody	= 0;	
	_LightPostionVehicalBody = 0;	
	_LSAmbientVehicalBody	 = 0;	
	_LSDiffuseVehicalBody	 = 0;	
	_LSSpecularVehicalBody	 = 0;	
	_MaterialAmbientVehicalBody = 0;	
	_MaterialDiffuseVehicalBody = 0;	
	_MaterialSpecularVehicalBody = 0;
						
}
//
void KrazyShadedVehicalRendrer::on_resize(int width , int height)
{
	
	_Width = width;_Height = height;
	float aspect = float(_Width)/float(_Height);
	float num    =  2.0f;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;
	_WindowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 1.0f , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	_ProjectionMatrix = glm::ortho(left , right , bottom , top , near_p , far_p);
}
//
void KrazyShadedVehicalRendrer::create()
{
// Start -- Vehical Body .............................................................................................
	char vertex_shader_blinn_phong[] = 
	"attribute vec3 inputPosition;\n"
	"attribute vec2 inputTexCoord;\n"
	"attribute vec3 inputNormal;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 window_scale;\n"
	"uniform mat4 modelview;\n"
	"uniform mat4 normalMat;\n"
	"uniform mat4 translate;\n"
	"uniform mat4 rotation;\n"
	"uniform mat4 lookat;\n"
	"varying vec3 normalInterp;\n"
	"varying vec3 vertPos;\n"
	"void main(){\n"
		";\n"
		"vec4 vertPos4 =  modelview * vec4(inputPosition , 1.0);\n"
		"vertPos = vec3(vertPos4)/vertPos4.w;\n"
		"normalInterp = vec3(modelview * vec4(inputNormal,0.0));\n"
		"gl_Position  = projection * window_scale * modelview * vec4(inputPosition , 1.0);\n"
		
	"}\n";
	//
	char fragment_shader_blinn_phong[] = 
	"uniform  mat4 fragMVMat;\n"
	"uniform  vec4 lightPos;\n"
	"varying  vec3 normalInterp;\n"
	"varying  vec3 vertPos;\n"
	"uniform int  mode;"
	"uniform  vec4 uLightAmbient;\n"      //light ambient property
	"uniform  vec4 uLightDiffuse;\n"      //light diffuse property 
	"uniform  vec4 uLightSpecular;\n"     //light specular property
 	"uniform  vec4 uMaterialAmbient;\n"  //object ambient property
	"uniform  vec4 uMaterialDiffuse;\n"   //object diffuse property
	"uniform  vec4 uMaterialSpecular;\n"  //object specular property
	"const  vec3 ambientColor = vec3(0.1,0.1,0.0);\n"
	"const  vec3 diffuseColor = vec3(0.5,0.5,0.0);\n"
	"const  vec3 specColor = vec3(1.0,1.0,1.0);\n"
	"void main(void){\n"
	 " vec4 Ia = uLightAmbient * uMaterialAmbient;\n"
     " vec4 Id = vec4(0.0,0.0,0.0,1.0);\n"
     " vec4 Is = vec4(0.0,0.0,0.0,1.0);\n"
	 " vec3 normal = normalize(normalInterp);"
	 " vec4 light = lightPos - vec4(vertPos,1.0);\n"
	 " vec3 lightDir = normalize(vec3(fragMVMat * lightPos));\n"
	 " float lambertian = max(dot(lightDir,normal) , 0.0);\n"
	 " float specular = 0.0;\n"
	 "if(lambertian > 0.0){\n"
		 "vec3 viewDir = normalize(-vertPos);\n"
		 "Id = uLightDiffuse * uMaterialDiffuse * lambertian;\n" //add diffuse term
#if 0
		"vec3 reflectDir = reflect(-lightDir , viewDir);\n"
		"float specAngle = max(dot(reflectDir , normal) , 0.0);\n"
#else
		"vec3 halfDir = normalize(lightDir + viewDir);\n"
		"float specAngle = max(dot(halfDir , normal) , 0.0);\n"
#endif
#if 0
		"if(specAngle == 0.0){\n"
			"specAngle = max(dot(-halfDir , normal) , 0.0);\n"
			"viewDir = normalize(vertPos);\n"
			"halfDir = normalize(lightDir + viewDir);\n"
			"specAngle = max(dot(halfDir , normal) , 0.0);\n"
		"}\n"
#endif
	  "specular = pow(specAngle , 5050.0);\n"
	  "Is = uLightSpecular * uMaterialSpecular * specular;\n" //add specular term
	  "gl_FragColor = Ia + Id + Is;//vec4(ambientColor + lambertian * diffuseColor + specular * specColor , 1.0);\n"
	   "}\n"
	   "else{\n"
		"gl_FragColor = vec4(1.0,1.0,0.0,1.0);\n"
	   "}\n"
	"}\n";
    
    KrazyShader vertexShader;vertexShader.LoadShader(GL_VERTEX_SHADER,vertex_shader_blinn_phong);
    KrazyShader fragmentShader;fragmentShader.LoadShader(GL_FRAGMENT_SHADER,fragment_shader_blinn_phong);
	//
	_ProgrammeVehicalBody.create(vertexShader,fragmentShader);
	_ProgrammeVehicalBody.compile();
	//
	_PositionVehicalBody		= glGetAttribLocation(_ProgrammeVehicalBody, "inputPosition");
	_NormalVehicalBody			= glGetAttribLocation(_ProgrammeVehicalBody,   "inputNormal");
	_ProjectVehicalBody			= glGetUniformLocation(_ProgrammeVehicalBody, "projection");
	_WindowScaleVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody, "window_scale");
	_ModelViewVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody, "modelview");
	_ModelFragVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody, "fragMVMat");
	_LightPostionVehicalBody	= glGetUniformLocation(_ProgrammeVehicalBody, "lightPos");
	_LSAmbientVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody,"uLightAmbient");
	_LSDiffuseVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody,"uLightDiffuse");
	_LSSpecularVehicalBody		= glGetUniformLocation(_ProgrammeVehicalBody,"uLightSpecular");
	_MaterialAmbientVehicalBody	= glGetUniformLocation(_ProgrammeVehicalBody,"uMaterialAmbient");
	_MaterialDiffuseVehicalBody	= glGetUniformLocation(_ProgrammeVehicalBody,"uMaterialDiffuse");
	_MaterialSpecularVehicalBody = glGetUniformLocation(_ProgrammeVehicalBody,"uMaterialSpecular");
	
//.............................................. Vehical Body End............................................................................................
}
//
void KrazyShadedVehicalRendrer::render(KrazyVehical* _vehical)
{

	KrazyVehicalShade* _vehical_shade = _vehical->shade();
	glm::vec2* _vec = _vehical->Vector();
	glUseProgram(_ProgrammeVehicalBody);
	glm::mat4 view( 1.0f , 0.0f , 0.0f , 0.0f, 
					0.0f , 1.0f , 0.0f , 0.0f, 
					0.0f , 0.0f , 1.0f , 0.0f, 
					_vec->x , _vec->y , 0.0 , 1.0f
					);
	float angle = _vehical->Angle();
	angle *= DEG_2_RAD;
	glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
    //
	float scalex = 1.0f;
	float scaley = 1.0f;
	float h = float(_Height);
	float w = float(_Width);
	float aspect = w/h;
	switch(_vehical->scaleorientation())
	{
		case KrazyScale_X: scalex = 1.0f;scaley = aspect; break;
		case KrazyScale_Y: scalex = aspect;scaley = 1.0f; break;	
	}
	//
	glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
					0.0f , scaley , 0.0f , 0.0f,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
	//
	glm::mat4 translate = view;
	glm::mat4 rotation  = model;
	glm::mat4 scale_fragment = scale;
	glm::mat4 model_fragment = model;
	glm::mat4 modelview = view * model * scale; 
	//
	glUniformMatrix4fv(_ProjectVehicalBody, 1, false, glm::value_ptr(_ProjectionMatrix));
	glUniformMatrix4fv(_WindowScaleVehicalBody, 1, false, glm::value_ptr(_WindowScaleMatrix));
	glUniformMatrix4fv(_ModelViewVehicalBody, 1, false, glm::value_ptr(modelview));
	glUniformMatrix4fv(_ModelFragVehicalBody , 1 , false , glm::value_ptr(model_fragment));
	glUniform4fv(_LightPostionVehicalBody ,   1 , glm::value_ptr(_vehical_shade->LSPosition()));
	glUniform4fv(_LSAmbientVehicalBody,       1 , glm::value_ptr(_vehical_shade->LSAmbient()));
	glUniform4fv(_LSDiffuseVehicalBody,       1 , glm::value_ptr(_vehical_shade->LSDiffuse()));
	glUniform4fv(_LSSpecularVehicalBody,      1 , glm::value_ptr(_vehical_shade->LSSpecular()));
	glUniform4fv(_MaterialAmbientVehicalBody, 1 , glm::value_ptr(_vehical_shade->MaterialAmbient()));
	glUniform4fv(_MaterialDiffuseVehicalBody, 1 , glm::value_ptr(_vehical_shade->MaterialDiffuse()));
	glUniform4fv(_MaterialSpecularVehicalBody,1 , glm::value_ptr(_vehical_shade->MaterialSpecular()));
	//
	glVertexAttribPointer(_PositionVehicalBody, 3, GL_FLOAT, GL_FALSE, 0, _vertex_buffer_data);
	glEnableVertexAttribArray(_PositionVehicalBody);
	glVertexAttribPointer(_NormalVehicalBody, 3, GL_FLOAT, GL_FALSE, 0, _vert_normal);
	glEnableVertexAttribArray(_NormalVehicalBody);
	//
	glDrawArrays(GL_TRIANGLES , 0 , 12);
	//
	glDisableVertexAttribArray(_PositionVehicalBody);
	glDisableVertexAttribArray(_NormalVehicalBody);
	//
	switch(_vehical->shadertype()){
	case KRAZY_SHADER_Sherrif:{
		if(_vehical->wait()++ == MAX_WAIT){
			_vehical->wait() = 0;
			_vehical->swap_lights();
		}
		KrazyVehicalShade* left_shade = _vehical->policelight_1();
		glUniform4fv(_LightPostionVehicalBody ,   1 , glm::value_ptr(left_shade->LSPosition()));
		glUniform4fv(_LSAmbientVehicalBody,       1 , glm::value_ptr(left_shade->LSAmbient()));
		glUniform4fv(_LSDiffuseVehicalBody,       1 , glm::value_ptr(left_shade->LSDiffuse()));
		glUniform4fv(_LSSpecularVehicalBody,      1 , glm::value_ptr(left_shade->LSSpecular()));
		glUniform4fv(_MaterialAmbientVehicalBody, 1 , glm::value_ptr(left_shade->MaterialAmbient()));
		glUniform4fv(_MaterialDiffuseVehicalBody, 1 , glm::value_ptr(left_shade->MaterialDiffuse()));
		glUniform4fv(_MaterialSpecularVehicalBody,1 , glm::value_ptr(left_shade->MaterialSpecular()));

		glVertexAttribPointer(_PositionVehicalBody, 3, GL_FLOAT, GL_FALSE, 0, _police_light);

		glEnableVertexAttribArray(_PositionVehicalBody);
		glVertexAttribPointer(_NormalVehicalBody, 3, GL_FLOAT, GL_FALSE, 0, _police_light_normals/*_vehical_shader->NormalList().data()*/);
		glEnableVertexAttribArray(_NormalVehicalBody);

		glDrawArrays(GL_TRIANGLES , 0 , 6);

		glDisableVertexAttribArray(_PositionVehicalBody);
		glDisableVertexAttribArray(_NormalVehicalBody);
		}
		break;
	}
}
//

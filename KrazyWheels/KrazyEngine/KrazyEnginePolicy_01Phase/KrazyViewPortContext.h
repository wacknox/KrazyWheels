#pragma once
#include "KrazyGLMInclusion.h"
class KrazyViewPortContext
{
public:
	KrazyViewPortContext(void);
	~KrazyViewPortContext(void);

	static int _Width;
	static int _Height;
	static glm::mat4 _ProjectionMatrix;
	static glm::mat4 _ViewMatrix;
	static glm::mat4 _ScreenScale;

};

class KrazyCaertsianConverter
{
public :

	static float get_x(int X , int Width);
	static float get_y(int Y , int Height);
};



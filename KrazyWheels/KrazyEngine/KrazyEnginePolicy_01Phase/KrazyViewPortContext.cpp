#include "StdAfx.h"
#include "KrazyViewPortContext.h"
//
int KrazyViewPortContext::_Width  = 0;
int KrazyViewPortContext::_Height = 0;
glm::mat4 KrazyViewPortContext::_ProjectionMatrix = glm::mat4();
glm::mat4 KrazyViewPortContext::_ViewMatrix = glm::mat4();
glm::mat4 KrazyViewPortContext::_ScreenScale = glm::mat4();
KrazyViewPortContext::KrazyViewPortContext(void){}
KrazyViewPortContext::~KrazyViewPortContext(void){}

float KrazyCaertsianConverter::get_x(int X , int Width)
{
	float N = float(2 * X);
	float D = float(Width);
	float R = N/D - float(1);
	return R;
}

float KrazyCaertsianConverter::get_y(int Y , int Height)
{
	float N = float(-2 * Y);
	float D = float(Height);
	float R = N/D + float(1);
	return R;
}
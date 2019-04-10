#pragma once
#include <wincodec.h>
#include "PlanetWindow.h"

#define SQUARE(x) ((x) * (x))

class VisibleObject
{
protected:
	Graphics* gfx; //Reference to the Graphics class	
public:
	bool isTouching(VisibleObject *planet);
	bool isTouching(floatPOINT *, int, floatPOINT *, int);

	float width, height;
	floatPOINT *anchorPoint = new floatPOINT();
	floatPOINT *location = new floatPOINT();

	//Draw bitmap to the render target
	void Draw(floatPOINT);
	void Draw();
};
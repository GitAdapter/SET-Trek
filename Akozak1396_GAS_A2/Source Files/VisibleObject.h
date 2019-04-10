#pragma once
#include <wincodec.h>
#include "PlanetWindow.h"

#define SQUARE(x) ((x) * (x))

class VisibleObject
{
protected:
	Graphics* gfx = nullptr; //Reference to the Graphics class	
public:
	bool isTouching(VisibleObject *planet);
	bool isTouching(floatPOINT *, int, floatPOINT *, int);

	float width = 0, height = 0;
	floatPOINT *anchorPoint = new floatPOINT();
	floatPOINT *location = new floatPOINT();
};
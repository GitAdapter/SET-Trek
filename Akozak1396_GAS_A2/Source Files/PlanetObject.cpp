#pragma once
#include "PlanetObject.h"

PlanetObject::PlanetObject(AnimationObject* an) : AnimationObject(an)
{
	repeatTimes = -1;
	frameSpeed = rand() % 10 + 10;
	/*for (auto i = bmp.begin(); i != bmp.end(); i++)
	{
		((ID2D1Bitmap*)*i)->Release();
	}*/
}

PlanetObject::~PlanetObject()
{
}
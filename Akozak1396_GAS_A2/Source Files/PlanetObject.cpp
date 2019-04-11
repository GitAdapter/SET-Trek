#pragma once
#include "VisibleObject.h"

PlanetObject::PlanetObject(AnimationObject* an) : AnimationObject(an)
{
	repeatTimes = -1;
	frameSpeed = rand() % 10 + 10;
}

PlanetObject::~PlanetObject()
{
}
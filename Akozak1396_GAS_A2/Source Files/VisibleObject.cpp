#include "VisibleObject.h"

bool VisibleObject::isTouching(VisibleObject *planet)
{
	bool isTouch = false;
	VisibleObject *ship = this;

	int sRad = ship->width / 2;
	int pRad = planet->width / 2;

	POINT sCenter;
	sCenter.x = (ship->width / 2) + ship->location->x;
	sCenter.y = (ship->height / 2) + ship->location->y;

	POINT pCenter;
	pCenter.x = (planet->width / 2) + planet->location->x;
	pCenter.y = (planet->height / 2) + planet->location->y;

	double distance = sqrt(SQUARE(pCenter.x - sCenter.x) + SQUARE(pCenter.y - sCenter.y));

	if (distance <= sRad + pRad)
	{
		isTouch = true;
	}

	return isTouch;
}

bool VisibleObject::isTouching(floatPOINT *fp1, int w1, floatPOINT *fp2, int w2)
{
	bool isTouch = false;

	int sRad = w2 / 2;
	int pRad = w1 / 2;

	POINT pCenter;
	pCenter.x = w1 + fp1->x;
	pCenter.y = w1 + fp1->y;

	POINT sCenter;
	sCenter.x = (w2 / 2) + fp2->x;
	sCenter.y = (w2 / 2) + fp2->y;

	double distance = sqrt(SQUARE(pCenter.x - sCenter.x) + SQUARE(pCenter.y - sCenter.y));

	if (distance <= sRad + pRad)
	{
		isTouch = true;
	}

	return isTouch;
}
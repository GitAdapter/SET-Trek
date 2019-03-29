#pragma once
#include "PlanetWindow.h"
#include <wincodec.h>

#define SQUARE(x) ((x) * (x))

class MovableObject
{
	Graphics* gfx; //Reference to the Graphics class	
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	const float baseShipSpeed = 10.5f;

public:
	//Constructor
	MovableObject();
	MovableObject(const wchar_t* filename, Graphics* gfx, bool isS, floatPOINT*, int numRows = 10, int numColumns = 10);

	void getShipSpeed(floatPOINT, floatPOINT, floatPOINT *, bool);
	bool isTouching(MovableObject *planet);
	void moveObject(bool isEnemy);

	float width, height, angle;

	int energyLevel = 0;
	int scienceLevel = 0;

	PlanetWindow *planetWindow = nullptr;

	floatPOINT *anchorPoint = new floatPOINT;

	floatPOINT *desintation = new floatPOINT();
	floatPOINT *location = new floatPOINT();
	floatPOINT *speed;

	//Destructor
	~MovableObject();

	//Draw bitmap to the render target
	void Draw(floatPOINT, bool shouldChroma = true, float rotation = 0.0f, D2D1_VECTOR_3F vector = { 0.0f, 1.0f, 0.0f });
};


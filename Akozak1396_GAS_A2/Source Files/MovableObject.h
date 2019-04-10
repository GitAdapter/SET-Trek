#pragma once
#include "PlanetWindow.h"
#include <wincodec.h>

#define SQUARE(x) ((x) * (x))

class MovableObject
{
	Graphics* gfx; //Reference to the Graphics class	
	const float baseShipSpeed = 10.5f;

public:
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	ID2D1Effect *chromakeyEffect;
	//Constructor
	MovableObject();
	MovableObject(MovableObject *m);
	MovableObject(const wchar_t* filename, Graphics* gfx, bool isS, floatPOINT*, D2D1_VECTOR_3F = { 0.0f, 1.0f, 0.0f }, int numRows = 10, int numColumns = 10);

	void getShipSpeed(floatPOINT, floatPOINT, floatPOINT *, bool);
	bool isTouching(MovableObject *planet);
	void moveObject(bool isEnemy);

	float width, height, angle;

	PlanetWindow *planetWindow = nullptr;

	floatPOINT *anchorPoint = new floatPOINT();

	floatPOINT *desintation = new floatPOINT();
	floatPOINT *location = new floatPOINT();
	floatPOINT *speed;

	//Destructor
	~MovableObject();

	//Draw bitmap to the render target
	void Draw(floatPOINT, bool shouldChroma = true, float rotation = 0.0f);
	void Draw();
};


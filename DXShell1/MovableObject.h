#pragma once
#include "PlanetWindow.h"

#include <wincodec.h> //This is the WIC codec header - we need this to decode image files
//Remember to add "windowscodecs.lib" to your Linker/Input/AdditionalDependencies

#define SQUARE(x) ((x) * (x))

class MovableObject
{
	Graphics* gfx; //Reference to the Graphics class	
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	const float baseShipSpeed = 2.5f;

public:
	//Constructor
	MovableObject();
	MovableObject(wchar_t* filename, Graphics* gfx, bool isS, int numRows = 10, int numColumns = 10);

	void getShipSpeed(POINTF, POINTF, POINTF *, bool);
	bool isTouching(MovableObject *planet);
	void moveObject(bool isEnemy);

	float width, height, angle;

	int energyLevel = 0;
	int scienceLevel = 0;

	PlanetWindow *planetWindow = nullptr;

	POINTF *desintation = new POINTF();
	POINTF *location = new POINTF();
	POINTF *speed = new POINTF();

	//Destructor
	~MovableObject();

	//Draw bitmap to the render target
	void Draw(POINTF, bool shouldChroma = true, float rotation = 0.0f, D2D1_VECTOR_3F vector = { 0.0f, 1.0f, 0.0f });
};
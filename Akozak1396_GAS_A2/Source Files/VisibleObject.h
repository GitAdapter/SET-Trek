#define _USE_MATH_DEFINES

#pragma once
#include <vector>
#include <d2d1_1.h>
#include <d2d1effects_2.h>
#include <math.h>
#include <wincodec.h>
#include "floatPoint.h"

#define SQUARE(x) ((x) * (x))

class Graphics;

class VisibleObject
{
protected:
	Graphics* gfx = nullptr; //Reference to the Graphics class	
	ID2D1RenderTarget* renderTarget;
	ID2D1DeviceContext* deviceContext;
public:
	VisibleObject* getVisibleObject() { return this; }
	bool isTouching(VisibleObject *planet);
	bool isTouching(floatPOINT *, int, floatPOINT *, int);

	float width = 0, height = 0;
	floatPOINT *anchorPoint = new floatPOINT();
	floatPOINT *location = new floatPOINT();
};

class MovableObject : public VisibleObject
{
private:
	const float baseShipSpeed = 5.0f;
public:
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	ID2D1Effect* chromakeyEffect;
	//Constructor
	MovableObject(MovableObject* m);
	MovableObject(const wchar_t* filename, ID2D1RenderTarget* rt, ID2D1DeviceContext* dc, bool isS, floatPOINT*, D2D1_VECTOR_3F = { 0.0f, 1.0f, 0.0f }, int numRows = 10, int numColumns = 10);

	void getShipSpeed(floatPOINT, floatPOINT, floatPOINT*, bool);
	void moveObject(bool isEnemy);
	float angle;

	floatPOINT* desintation;
	floatPOINT* speed;

	//Destructor
	~MovableObject();

	//Draw bitmap to the render target
	void Draw(floatPOINT, bool shouldChroma = true, float rotation = 0.0f);
	void Draw();
};


class AnimationObject : public VisibleObject
{
public:
	std::vector<ID2D1Bitmap*> bmp; //This will hold our loaded and converted Bitmap file
	//Constructor
	AnimationObject(AnimationObject* m);
	AnimationObject(const wchar_t* filename, ID2D1RenderTarget* rt, ID2D1DeviceContext* dc, floatPOINT*, int numRows, int numColumns, int frameRows, int frameColumns, int framesPerRender = 1, int repeat = 1);

	int currentFrame = 0, frameSpeed;
	int repeatTimes = 1, currTimes = 0;
	bool completedAnimation = false;

	//Destructor
	~AnimationObject();

	//Draw bitmap to the render target
	void Draw();
	floatPOINT Draw(floatPOINT);
};

class PlanetObject : public AnimationObject
{
public:
	PlanetObject(AnimationObject* m);
	void* planetWindow;
	//Destructor
	~PlanetObject();
};
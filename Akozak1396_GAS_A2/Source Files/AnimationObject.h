#include "PlanetWindow.h"
#include <wincodec.h>

#define SQUARE(x) ((x) * (x))

class AnimationObject
{
	Graphics* gfx; //Reference to the Graphics class	
	const float baseShipSpeed = 10.5f;

public:
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	//Constructor
	AnimationObject();
	AnimationObject(AnimationObject *m);
	AnimationObject(std::vector<const wchar_t*> filename, Graphics* gfx, floatPOINT*, int numRows = 10, int numColumns = 10);

	void getShipSpeed(floatPOINT, floatPOINT, floatPOINT *, bool);

	float width, height, angle, frameSpeed, numFrames;

	PlanetWindow *planetWindow = nullptr;

	floatPOINT *anchorPoint = new floatPOINT();

	floatPOINT *desintation = new floatPOINT();
	floatPOINT *location = new floatPOINT();
	floatPOINT *speed;

	//Destructor
	~AnimationObject();

	//Draw bitmap to the render target
	void Draw(floatPOINT, bool shouldChroma = true, float rotation = 0.0f, D2D1_VECTOR_3F vector = { 0.0f, 1.0f, 0.0f });
};


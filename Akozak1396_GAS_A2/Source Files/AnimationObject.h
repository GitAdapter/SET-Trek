#include "PlanetWindow.h"
#include <wincodec.h>

#define SQUARE(x) ((x) * (x))

class AnimationObject
{
	Graphics* gfx; //Reference to the Graphics class	
	const float baseShipSpeed = 10.5f;

public:
	std::vector<ID2D1Bitmap*> bmp; //This will hold our loaded and converted Bitmap file
	//Constructor
	AnimationObject();
	AnimationObject(AnimationObject *m);
	AnimationObject(const wchar_t* filename, Graphics* gfx, floatPOINT*, int numRows, int numColumns, int frameRows, int frameColumns);

	void getShipSpeed(floatPOINT, floatPOINT, floatPOINT *, bool);

	float width, height, angle;
	int currentFrame = 0, frameSpeed = 60, numFrames;

	PlanetWindow *planetWindow = nullptr;

	floatPOINT *anchorPoint = new floatPOINT();

	floatPOINT *desintation = new floatPOINT();
	floatPOINT *location = new floatPOINT();
	floatPOINT *speed;

	bool completedAnimation = false;

	//Destructor
	~AnimationObject();

	//Draw bitmap to the render target
	void Draw();
};


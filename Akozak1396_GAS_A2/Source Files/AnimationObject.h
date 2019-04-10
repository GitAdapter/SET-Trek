#include "PlanetWindow.h"
#include "VisibleObject.h"
#include <wincodec.h>

class AnimationObject : public VisibleObject
{
public:
	std::vector<ID2D1Bitmap*> bmp; //This will hold our loaded and converted Bitmap file
	//Constructor
	AnimationObject();
	AnimationObject(AnimationObject *m);
	AnimationObject(const wchar_t* filename, Graphics* gfx, floatPOINT*, int numRows, int numColumns, int frameRows, int frameColumns, int framesPerRender = 1);

	int currentFrame = 0, frameSpeed, numFrames;
	int repeatTimes = 1, currTimes = 0;
	bool completedAnimation = false;

	//Destructor
	~AnimationObject();

	//Draw bitmap to the render target
	void Draw();
	floatPOINT Draw(floatPOINT);
};


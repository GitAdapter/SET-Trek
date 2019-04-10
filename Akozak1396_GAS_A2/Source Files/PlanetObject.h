#include "PlanetWindow.h"
#include "AnimationObject.h"
#include <wincodec.h>

#define SQUARE(x) ((x) * (x))

class PlanetObject : public AnimationObject
{
public:
	PlanetObject(AnimationObject *m);
	PlanetWindow *planetWindow = new PlanetWindow();
	//Destructor
	~PlanetObject();
};


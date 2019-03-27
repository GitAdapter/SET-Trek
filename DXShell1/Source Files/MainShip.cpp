#include "Graphics.h"
#include "GameController.h"
#include "GameLevel.h"

class ship
{
public:
	ship(Graphics);
	~ship();

private:
	Sprite* sb;
	Sprite* sd;

};

ship::ship(Graphics gfx)
{
	sb = new Sprite(L"ShipBase.bmp", gfx); //This is where we can specify our file system object!
	sd = new Sprite(L"ShipDetail.bmp", gfx); //This is where we can specify our file system object!
}

ship::~ship()
{
}
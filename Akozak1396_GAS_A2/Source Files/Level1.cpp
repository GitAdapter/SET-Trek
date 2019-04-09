#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include "GameOver.h"

#define UNIVERSE_WIDTH 100
#define UNIVERSE_HEIGHT UNIVERSE_WIDTH
#define CHANCE_OF_PLANET 25
#define ENEMY_DISTANCE 500

void Level1::Load() 
{
	if (gfx->Energy == NULL)
	{
		gfx->Energy = new int;
		*gfx->Energy = 300;

		gfx->Science = new int;
		*gfx->Science = 0;
	}

	playerShip = new MovableObject(L"Resources\\images\\1.png", gfx, true, new floatPOINT()); //This is where we can specify our file system object!

	ID2D1RenderTarget *rt = gfx->GetRenderTarget();

	*anchor = *playerShip->location;
	shipPosition.x = rt->GetSize().width / 2 + 1;
	shipPosition.y = rt->GetSize().height / 2;

	background = new MovableObject(L"Resources\\images\\SectorBackground.bmp", gfx, false, anchor, 1, 1); //This is where we can specify our file system object!
	background->location = new floatPOINT{ 0.0f, 0.0f };
	background->anchorPoint = background->location;

	basePlanet1 = new MovableObject(L"Resources\\images\\Planet1.bmp", gfx, false, anchor); //This is where we can specify our file system object!
	basePlanet2 = new MovableObject(L"Resources\\images\\Planet2.bmp", gfx, false, anchor); //This is where we can specify our file system object!
	basePlanet3 = new MovableObject(L"Resources\\images\\Planet3.bmp", gfx, false, anchor); //This is where we can specify our file system object!
	playerDetails = new MovableObject(L"Resources\\images\\ShipDetail.bmp", gfx, true, playerShip->anchorPoint); //This is where we can specify our file system object!
	enemyPointer = new MovableObject(L"Resources\\images\\EnemyDirection.bmp", gfx, true, playerShip->anchorPoint, 5, 5); //This is where we can specify our file system object!
	enemyShip = new MovableObject(L"Resources\\images\\EnemyShip.bmp", gfx, false, anchor);
	
	enemyShip->speed = new floatPOINT{ 1.0f, 1.0f };
	playerShip->speed = new floatPOINT{ 0.0f, 0.0f };

	resetBoard();
	*gfx->destination = *playerShip->location;
}


void Level1::Unload()
{
	delete background;
}

void Level1::Update()
{
	gfx->canDrag = true;
	if (gfx->destination->x != -1)
	{
		*playerShip->desintation = *playerShip->location + *gfx->destination;
		playerShip->desintation->x -= shipPosition.x;
		playerShip->desintation->y -= shipPosition.y;
		gfx->destination->x = -1;
	}	
	bool isMoving = !(playerShip->desintation->x == playerShip->location->x);
	*anchor = *playerShip->location;

	if (isMoving)
	{
		enemyShip->moveObject(true);
		playerShip->moveObject(false);
	}
}

void Level1::resetBoard()
{
	repopSector();
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	delete(playerShip->location);
	playerShip->location = new floatPOINT(shipPosition);
	*playerShip->desintation = *playerShip->location;
	playerShip->angle = 0.0f;
	*anchor = *playerShip->location;

	enemyShip->location->x = ENEMY_DISTANCE + anchor->x;
	enemyShip->location->y = ENEMY_DISTANCE + anchor->y;
	enemyShip->desintation = playerShip->location;

	int choicex = rand() % 100;
	int choicey = rand() % 100;

	if (choicex % 2 == 0) { enemyShip->location->x = -ENEMY_DISTANCE + anchor->x; }
	if (choicey % 2 == 0) { enemyShip->location->y = -ENEMY_DISTANCE + anchor->y; }

	*gfx->destination = *playerShip->location;	
	enemyShip->moveObject(true);
	Render();
}

void Level1::Render()
{
	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();

	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	background->Draw({ 0.0f, 0.0f }, false);

	playerShip->Draw(shipPosition, true, playerShip->angle);
	enemyPointer->Draw(shipPosition, true, enemyShip->angle + 180);
	enemyShip->Draw(*enemyShip->location + shipPosition, true, enemyShip->angle + 180, { 0.0f, 0.0f, 1.0f });

	if (((int)playerShip->location->x % (int)(windowSize.width / 20)) == 0  || ((int)playerShip->location->y % (int)(windowSize.height / 20)) == 0)
	{		
		float xMAX = playerShip->location->x + (windowSize.width * 1.5) - shipPosition.x;
		float xMIN = playerShip->location->x - shipPosition.x - 500;
		float yMAX = playerShip->location->y + (windowSize.height * 1.5) - shipPosition.y;
		float yMIN = playerShip->location->y - shipPosition.y - 500;
		
		onScreenPlanets.clear();

		for (auto i = currSprites.begin(); i != currSprites.end(); i++)
		{
			Planet s = *i;
			float x = i->obj->location->x, y = i->obj->location->y;
			if (x > xMIN && x < xMAX && y > yMIN && y < yMAX)
			{
				onScreenPlanets.push_back(s);
				if (s.obj->planetWindow == nullptr)
				{
					s.obj->planetWindow = new PlanetWindow();
				}
			}
		}
	}

	for (auto i = onScreenPlanets.begin(); i != onScreenPlanets.end(); i++)
	{
		Planet s = *i;
		s.obj->Draw(*s.obj->location + shipPosition, true, s.obj->angle);
		
		if (playerShip->isTouching(s.obj))
		{
			if (!s.obj->planetWindow->visited)
			{
				s.obj->planetWindow->visited = true;
				GameController::OpenPopUp(s.obj->planetWindow);
			}
			break;
		}
		else
		{
			s.obj->planetWindow->visited = false;
		}
	}

	//wchar_t CurrScienceString[40];
	//swprintf_s(CurrScienceString, L"Total Science: %d", *gfx->Science);

	//wchar_t CurrEnergyString[40];
	//swprintf_s(CurrEnergyString, L"Total Energy: %d", *gfx->Energy);

	wchar_t CurrScienceString[40];
	swprintf_s(CurrScienceString, L"Enemy: %0.2f, %0.2f", enemyShip->location->x, enemyShip->location->y);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Player: %0.2f, %0.2f; %0.2f", playerShip->location->x, playerShip->location->y, sqrtf(pow(playerShip->speed->x,2) + pow(playerShip->speed->y, 2)));

	gfx->DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	gfx->DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	gfx->DrawScreenText(CurrScienceString, 0, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 24);

	gfx->DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	gfx->DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 24);
	
	if (gfx->message != nullptr)
	{
		if (gfx->message->message != 0)
		{
			swprintf_s(CurrEnergyString, L"Mesage Number: %d", gfx->message->message);
			gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 60, windowSize.width / 2, 30, D2D1::ColorF::White, 24);
		}
	}
	
	if (playerShip->isTouching(enemyShip))
	{
		*gfx->Energy -= 300;
		if (*gfx->Energy <= 0 && false)
		{
			gfx->canDrag = false;
			GameController::SwitchLevel(new GameOver());
		}
		else
		{
			resetBoard();
		}
		gfx->isDragging = false;
	}
}

void Level1::UnPause()
{
	gfx->destination->x = -1;
	*playerShip->desintation = *playerShip->location;
	Render();
	Sleep(100);
}

void Level1::repopSector()
{
	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();

	

	currSprites.clear();

	for (int i = -UNIVERSE_HEIGHT; i < UNIVERSE_HEIGHT; i++)
	{
		for (int j = -UNIVERSE_WIDTH; j < UNIVERSE_WIDTH; j++)
		{
			if (rand() % CHANCE_OF_PLANET == 0)
			{
				int selPlanet = rand() % 3;
				Planet s;
				switch (selPlanet)
				{
				case 0:
					s.obj = new MovableObject(*basePlanet1);
					break;
				case 1:
					s.obj = new MovableObject(*basePlanet2);
					break;
				case 2:
					s.obj = new MovableObject(*basePlanet3);					
					break;
				}
				//delete(s.ss->location);
				s.obj->location = new floatPOINT{ (windowSize.width / 10) * i + s.obj->width / 2, (windowSize.height / 10) * j + s.obj->height / 2 };

				currSprites.emplace_back(s);
			}
		}
	}
}
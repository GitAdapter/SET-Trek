#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include "GameOver.h"

void Level1::Load() 
{
	if (gfx->Energy == NULL)
	{
		gfx->Energy = new int;
		*gfx->Energy = 300;

		gfx->Science = new int;
		*gfx->Science = 0;
	}

	background = new MovableObject(L"SectorBackground.bmp", gfx, false, 1, 1); //This is where we can specify our file system object!
	background->location = new POINTF{ 0.0f, 0.0f };
	basePlanet1 = new MovableObject(L"Planet1.bmp", gfx, false); //This is where we can specify our file system object!
	basePlanet2 = new MovableObject(L"Planet2.bmp", gfx, false); //This is where we can specify our file system object!
	basePlanet3 = new MovableObject(L"Planet3.bmp", gfx, false); //This is where we can specify our file system object!
	playerShip = new MovableObject(L"ShipBase.bmp", gfx, true); //This is where we can specify our file system object!
	playerDetails = new MovableObject(L"ShipDetail.bmp", gfx, true); //This is where we can specify our file system object!
	enemyShip = new MovableObject(L"EnemyShip.bmp", gfx, false);

	enemyShip->location = new POINTF{ gfx->GetRenderTarget()->GetSize().width - enemyShip->width, gfx->GetRenderTarget()->GetSize().height / 2 };
	enemyShip->desintation = playerShip->location;
	
	resetBoard();
	*gfx->destination = *playerShip->location;
}


void Level1::Unload()
{
	delete background;
}

void Level1::Update()
{
	playerShip->desintation = gfx->destination;
	bool isMoving = !(playerShip->desintation->x == playerShip->location->x);

	if (isMoving)
	{
		enemyShip->moveObject(true);
		playerShip->moveObject(false);
	}
}

void Level1::resetBoard()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	delete(playerShip->location);
	playerShip->location = new POINTF{ playerShip->width / 2 + 1, rt->GetSize().height / 2 };
	playerShip->desintation = playerShip->location;
	playerShip->angle = 0.0f;

	delete(enemyShip->location);
	enemyShip->location = new POINTF{ rt->GetSize().width - enemyShip->width, rt->GetSize().height / 2 };
	enemyShip->desintation = playerShip->location;
	enemyShip->angle = 0.0f;

	*gfx->destination = *playerShip->location;
	repopSector();
}

void Level1::Render()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	D2D1_SIZE_F windowSize = rt->GetSize();

	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	background->Draw({ 0.0f, 0.0f }, false);


	playerShip->Draw(*playerShip->location, true, playerShip->angle);
	playerDetails->Draw(*playerShip->location, true, playerShip->angle);
	enemyShip->Draw(*enemyShip->location, true, enemyShip->angle, { 0.0f, 0.0f, 1.0f });

	for (auto i = currSprites.begin(); i != currSprites.end(); i++)
	{
		Planet s = *i;
		s.obj->Draw(*s.obj->location, true, s.obj->angle);
	}
	wchar_t CurrScienceString[40];
	swprintf_s(CurrScienceString, L"Total Science: %d", *gfx->Science);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Energy: %d", *gfx->Energy);

	gfx->DrawScreenText(CurrScienceString, 0, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 28);
	gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 28);
	checkPlanetCollision();
	
	if (playerShip->isTouching(enemyShip))
	{
		*gfx->Energy -= 300;
		if (*gfx->Energy <= 0)
		{
			GameController::SwitchLevel(new GameOver());
		}
		else
		{
			resetBoard();
		}
	}
}

void Level1::checkPlanetCollision()
{
	for (auto i = currSprites.begin(); i != currSprites.end(); i++)
	{
		Planet p = *i;
		if (p.obj->planetWindow == nullptr)
		{
			p.obj->planetWindow = new PlanetWindow();
		}

		if (playerShip->isTouching(p.obj))
		{
			if (!p.obj->planetWindow->visited)
			{
				p.obj->planetWindow->visited = true;
				GameController::OpenPopUp(p.obj->planetWindow);
			}
			break;
		}		
		else
		{
			p.obj->planetWindow->visited = false;
		}
	}
}

void Level1::UnPause()
{
	*gfx->destination = *playerShip->location;
	Render();
	Sleep(100);
}

void Level1::repopSector()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	D2D1_SIZE_F windowSize = rt->GetSize();
	currSprites.clear();
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (rand() % 20 == 0)
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
				s.obj->location = new POINTF{ (windowSize.width / 10) * i + s.obj->width / 2, (windowSize.height / 10) * j + s.obj->height / 2 };

				currSprites.push_back(s);
			}
		}
	}
}
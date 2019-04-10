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

	floatPOINT ORIGIN = { 0.0f, 0.0f };

	playerShip = new MovableObject(L"Resources\\images\\PlayerShip.png", gfx, true, new floatPOINT());

	ID2D1RenderTarget *rt = gfx->GetRenderTarget();

	*anchor = *playerShip->location;
	shipPosition.x = rt->GetSize().width / 2 + 1;
	shipPosition.y = rt->GetSize().height / 2;

	background = new MovableObject(L"Resources\\images\\bg.jpg", gfx, false, anchor, { 0,0,0 }, 0.05, 0.05);
	background->location =  &shipPosition;	
	*background->anchorPoint = *anchor;

	planets.push_back(new AnimationObject(L"Resources\\images\\p1.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p2.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p3.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p4.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p5.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p6.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p7.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p8.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p9.png", gfx, anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p10.png", gfx, anchor, 10, 10, 4, 4, 5));

	playerDetails = new MovableObject(L"Resources\\images\\ShipDetail.bmp", gfx, true, playerShip->anchorPoint); //This is where we can specify our file system object!
	enemyPointer = new MovableObject(L"Resources\\images\\EnemyDirection.bmp", gfx, true, playerShip->anchorPoint, { 0.0f, 1.0f, 0.0f }, 5, 5); //This is where we can specify our file system object!
	enemyShip = new MovableObject(L"Resources\\images\\EnemyShip.bmp", gfx, false, anchor, { 0.0f, 0.0f, 1.0f });
	
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe1.png", gfx, anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe2.png", gfx, anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe3.png", gfx, anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe4.png", gfx, anchor, 0, 0, 1, 60, 5, -1));

	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion1.png", gfx, anchor, 0, 0, 10, 7, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion2.png", gfx, anchor, 0, 0, 10, 8, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion3.png", gfx, anchor, 0, 0, 10, 9, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion4.png", gfx, anchor, 0, 0, 10, 8, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion5.png", gfx, anchor, 0, 0, 10, 7, 2));

	randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\ShootingStar.png", gfx, anchor, 0, 0, 1, 23, 2));
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(explosions[0]);
	randomEnvironment.push_back(explosions[1]);
	randomEnvironment.push_back(explosions[2]);
	randomEnvironment.push_back(explosions[3]);
	randomEnvironment.push_back(explosions[4]);
	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic.png", gfx, anchor, 0, 0, 7, 7, 2));
	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic2.png", gfx, anchor, 0, 0, 9, 9, 2));
	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic3.png", gfx, anchor, 0, 0, 9, 9, 2));

	//boxes = new AnimationObject(L"Resources\\images\\boxes.png", gfx, anchor, 2, 2, 6, 1, 8);
	//boxes = new AnimationObject(L"Resources\\images\\flame1.png", gfx, anchor, 2, 2, 3, 3, 3);
	boxes = new AnimationObject(L"Resources\\images\\chicken.png", gfx, anchor, 2, 2, 1, 41, 8);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       

	AnimationObject op = new AnimationObject(explosions[rand()%5]);
	op.repeatTimes = -1;
	op.location = new floatPOINT{ 150.0f, 150.0f };	

	animations.push_back(op);

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
	animations.clear();
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

bool isComplete(const AnimationObject& value) 
{ 
	return value.completedAnimation; 
}

void Level1::Render()
{
	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();

	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	background->Draw();

	if (rand() % 100 == 0)
	{
		AnimationObject ss = new AnimationObject(randomEnvironment[rand() % randomEnvironment.size()]);
		ss.location = new floatPOINT{ playerShip->location->x - (float)(rand() % (int)windowSize.width - (int)windowSize.width / 2),
			playerShip->location->y - (float)(rand() % (int)windowSize.height - (int)windowSize.height / 2) };
		ss.anchorPoint = anchor;
		animations.push_back(ss);
	}

	std::list<int> deleteIndecies;

	for (auto i = animations.begin(); i != animations.end(); i++)
	{
		(*i).Draw(shipPosition);
	}
	animations.remove_if(isComplete);

	playerShip->Draw(shipPosition, false, playerShip->angle);
	enemyPointer->Draw(shipPosition, true, enemyShip->angle + 180);
	enemyShip->Draw(*enemyShip->location + shipPosition, true, enemyShip->angle + 180);

	if (((int)playerShip->location->x % (int)(windowSize.width / 20)) == 0  || ((int)playerShip->location->y % (int)(windowSize.height / 20)) == 0)
	{		
		float xMAX = playerShip->location->x + (windowSize.width * 2);
		float xMIN = playerShip->location->x - (windowSize.width * 2);
		float yMAX = playerShip->location->y + (windowSize.height * 2);
		float yMIN = playerShip->location->y - (windowSize.height * 2);
		
		onScreenPlanets.clear();

		for (auto i = currSprites.begin(); i != currSprites.end(); i++)
		{
			PlanetObject s = *i;
			float x = i->location->x, y = i->location->y;
			if (x > xMIN && x < xMAX && y > yMIN && y < yMAX)
			{
				onScreenPlanets.push_back(s);
			}
		}
	}

	for (auto i = onScreenPlanets.begin(); i != onScreenPlanets.end(); i++)
	{
		floatPOINT fp = (*i).Draw(shipPosition);
		if (playerShip->isTouching(&fp, (*i).width, &shipPosition, playerShip->width))
		{			

			if (!(*i).planetWindow->visited)
			{
				if ((*i).planetWindow->planetAnimation == nullptr)
				{
					(*i).planetWindow->planetAnimation = globes[rand()%globes.size()];
				}
				(*i).planetWindow->visited = true;
				GameController::OpenPopUp((*i).planetWindow);
				break;
			}			
		}	
		else { (*i).planetWindow->visited = false; }
	}

	wchar_t CurrScienceString[40];
	swprintf_s(CurrScienceString, L"Total Science: %d", *gfx->Science);
	//swprintf_s(CurrScienceString, L"Enemy: %0.2f, %0.2f", enemyShip->location->x, enemyShip->location->y);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Energy: %d", *gfx->Energy);
	//swprintf_s(CurrEnergyString, L"Player: %0.2f, %0.2f; %0.2f", playerShip->location->x, playerShip->location->y, sqrtf(pow(playerShip->speed->x,2) + pow(playerShip->speed->y, 2)));

	gfx->DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	gfx->DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	gfx->DrawScreenText(CurrScienceString, 0, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 24);

	gfx->DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	gfx->DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 30, windowSize.width/2, 30, D2D1::ColorF::White, 24);
	
	//if (gfx->message != nullptr)
	//{
	//	if (gfx->message->message != 0)
	//	{
	//		swprintf_s(CurrEnergyString, L"Mesage Number: %d", gfx->message->message);
	//		gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 60, windowSize.width / 2, 30, D2D1::ColorF::White, 24);
	//	}
	//}
	
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
				PlanetObject p = new PlanetObject(planets[rand() % planets.size()]);
				p.location = new floatPOINT{ (windowSize.width / 10) * i + p.width / 2, (windowSize.height / 10) * j + p.height / 2 };
				p.anchorPoint = anchor;
				currSprites.emplace_back(p);
			}
		}
	}
}
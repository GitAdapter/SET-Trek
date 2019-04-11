#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include "PlanetWindow.h"
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

	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic.png", gfx, anchor, 0, 0, 7, 7, 2));
	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic2.png", gfx, anchor, 0, 0, 9, 9, 2));
	//randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\magic3.png", gfx, anchor, 0, 0, 9, 9, 2));

	//boxes = new AnimationObject(L"Resources\\images\\boxes.png", gfx, anchor, 2, 2, 6, 1, 8);
	//boxes = new AnimationObject(L"Resources\\images\\flame1.png", gfx, anchor, 2, 2, 3, 3, 3);	

	//gfx->enemyShip->speed = new floatPOINT{ 1.0f, 1.0f };
	//gfx->playerShip->speed = new floatPOINT{ 0.0f, 0.0f };
	//*gfx->destination = *gfx->playerShip->location;

	//resetBoard();
}

void Level1::addWindows()
{
	for (int i = 0; i < gfx->allPlanets.size(); i++)
	{
		((PlanetObject*)&gfx->allPlanets[i])->planetWindow = new PlanetWindow();
	}
}


void Level1::Unload()
{

}

void Level1::Update()
{
	if (!gfx->allPlanets[0].planetWindow) 
	{ 
		addWindows(); 
	}
	gfx->UpdateVisuals();

	for (int i = 0; i < gfx->onScreenPlanets.size(); i++)
	{
		PlanetObject* po = &(gfx->onScreenPlanets[i]);
		PlanetWindow* pw = (PlanetWindow*)po->planetWindow;
		if (gfx->playerShip->isTouching(po))
		{
			if (!pw->visited)
			{
				if (pw->planetAnimation == nullptr)
				{
					pw->planetAnimation = gfx->globes[rand() % gfx->globes.size()];
				}
				pw->visited = true;
				GameController::OpenPopUp(pw);
				*gfx->playerShip->desintation = *gfx->playerShip->location;
				break;
			}
		}
		else { pw->visited = false; }
	}
}

void Level1::resetBoard()
{
	gfx->RefreshSector();
	addWindows();
	Render();
}

void Level1::Render()
{
	gfx->RenderShipScreen();

	if (rand() % 100 == 0)
	{
		gfx->PlayRandomEffect();
	}

	if (gfx->playerShip->isTouching(gfx->enemyShip))
	{
		gfx->playerShip->isTouching(gfx->enemyShip);
		*gfx->Energy -= 300;
		if (*gfx->Energy <= 0 && false)
		{
			gfx->canDrag = false;
			GameController::SwitchLevel(new GameOver());
		}
		else
		{
			gfx->RefreshSector();
		}
		gfx->isDragging = false;
	}
}

void Level1::UnPause()
{
	gfx->destination->x = -1;
	Render();
	Sleep(100);
}
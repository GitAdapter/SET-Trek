/*
*  FILE          : MainGame.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to run the main game, mostly through calls and interactions
*	 with the Graphics class.
*/

#include "MainGame.h"
#include "GameOver.h"

#define DEFAULT_GRIDS 10

void MainGame::Load()
{
	if (gfx->Energy == NULL || *gfx->Energy <= 0)
	{
		gfx->Energy = new int;
		*gfx->Energy = 300;

		gfx->Science = new int;
		*gfx->Science = 0;
	}

	PlaySound(TEXT(soundLibrary[Sound::background]), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void MainGame::addWindows()
{
	for (int i = 0; i < gfx->allPlanets.size(); i++)
	{
		gfx->allPlanets[i]->planetWindow = new PlanetWindow();
	}
}

void MainGame::Unload()
{

}

void MainGame::Update()
{
	if (!gfx->allPlanets[0]->planetWindow) 
	{ 
		addWindows(); 
	}
	gfx->UpdateVisuals();

	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();

	for (int i = 0; i < gfx->onScreenPlanets.size(); i++)
	{
		if (gfx->onScreenPlanets[i]->frameSpeed != -1)
		{
			PlanetObject* po = gfx->onScreenPlanets[i];
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

			if (gfx->enemyShip->isTouching(po))
			{
				po->completedAnimation = true;
				po->currTimes = 0;
				po->repeatTimes = 0;
				floatPOINT* fp = new floatPOINT{ po->location->x - windowSize.width / (DEFAULT_GRIDS * 2), po->location->y - windowSize.height / (DEFAULT_GRIDS * 2) };
				gfx->CreateExplosion(*fp);

				char buffer[100];
				sprintf(buffer, "play \"%s\"", soundLibrary[Sound::explosion]);
				mciSendString(buffer, NULL, 0, NULL);

				gfx->onScreenPlanets[i]->frameSpeed = -1;
			}
		}
	}
}

void MainGame::resetBoard()
{
	gfx->RefreshSector();
	addWindows();
	Render();
}

void MainGame::Render()
{
	gfx->RenderShipScreen();

	if (rand() % 100 == 0)
	{
		gfx->PlayRandomEffect();
	}

	if (gfx->playerShip->isTouching(gfx->enemyShip))
	{
		char buffer[100];
		sprintf(buffer, "play \"%s\"", soundLibrary[Sound::explosion]);
		mciSendString(buffer, NULL, 0, NULL);

		gfx->playerShip->isTouching(gfx->enemyShip);
		*gfx->Energy -= 300;
		if (*gfx->Energy <= 0)
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

void MainGame::UnPause()
{
	gfx->destination->x = -1;
	Render();
	Sleep(100);
}
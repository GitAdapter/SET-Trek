/*
*  FILE          : GameOver.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to create the GameOver screen, and restart the game if there is
*	 is some kinf od click input from the user.
*/

#include "GameOver.h"

void GameOver::Load()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	D2D1_SIZE_F windowSize = rt->GetSize();
	gfx->ClearScreen(0, 0, 0);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Science Gathered: %d", *gfx->Science);

	gfx->DrawScreenText(CurrEnergyString, 0, windowSize.height - 100, windowSize.width, 100, D2D1::ColorF::Gold, 56);
	
	gfx->DrawScreenText(L"GAME OVER", 0, windowSize.height / 2 - 75, windowSize.width, 125, D2D1::ColorF::White, 125);
	gfx->DrawScreenText(L"Click to play again.", 0, windowSize.height / 2, windowSize.width, 125, D2D1::ColorF::White, 36);

	PlaySound(TEXT(soundLibrary[Sound::gameOver]), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	gfx->destination->x = -1;
}

void GameOver::Unload()
{

}

void GameOver::UnPause()
{

}

void GameOver::Render()
{

	if (gfx->destination->x != -1)
	{
		//GameController::LoadInitialLevel()
		gfx->RefreshSector();
		GameController::SwitchLevel(new MainGame());
	}
}

void GameOver::Update()
{

}
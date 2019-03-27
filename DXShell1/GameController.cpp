
#include "GameController.h"

GameLevel* GameController::currentLevel;
GameLevel* GameController::tempStorage;
bool GameController::Loading;

void GameController::Init()
{
	Loading = true;
	currentLevel = 0;
}

void GameController::LoadInitialLevel(GameLevel* lev)
{
	Loading = true; //This can help us avoid loading activity while rendering
	currentLevel = lev;
	currentLevel->Load();
	Loading = false;
}

void GameController::SwitchLevel(GameLevel* lev)
{
	Loading = true;
	currentLevel->Unload();
	lev->Load();
	delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameController::OpenPopUp(GameLevel* lev)
{
	Loading = true;
	lev->Load();
	tempStorage = currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameController::ClosePopUp()
{
	Loading = true;
	currentLevel = tempStorage;
	currentLevel->UnPause();
	Loading = false;
}

void GameController::Render()
{
	if (Loading) return;//nice! Do not update or render if the scene is loading.
	currentLevel->Render();
}

void GameController::Update()
{
	if (Loading) return; //nice! Do not update or render if the scene is loading.
	currentLevel->Update();
}
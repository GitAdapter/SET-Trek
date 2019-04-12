/*
*  FILE          : GameController.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to run the main game and realizes all of the functionality of
*	 the GameController class.
*/

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
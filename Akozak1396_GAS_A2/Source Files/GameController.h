/*
*  FILE          : GameController.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to define the GameController class, which is responsible for 
*	 posessing the current game level along with any pop-ups.
*/

#pragma once
#include "GameLevel.h"

#define _CRT_SECURE_NO_WARNINGS

#ifndef GRAPHICS_DEFINED
#include "Graphics.h"
#endif // !GRAPHICS_DEFINED

//This will be a Singleton class (constructor is private)

class GameController
{
	GameController() {}
	static GameLevel* currentLevel;
	static GameLevel* tempStorage;
public:
	static bool Loading;
	static void Init();
	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev);
	static void OpenPopUp(GameLevel* lev);
	static void ClosePopUp();
	static void Render();
	static void Update();
};
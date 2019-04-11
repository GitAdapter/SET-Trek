#pragma once

#include "GameLevel.h"

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
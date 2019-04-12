/*
*  FILE          : MainGame.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to initialize the MainGame class which is the main
*	 GameLevel for the game. This contains the main exploration among other things. 
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "GameLevel.h"
#include "Graphics.h"
#include "GameController.h"
#include "PlanetWindow.h"

class MainGame : public GameLevel
{
public:
	void Load() override;
	void Unload() override;
	void UnPause() override;
	void Update() override;
	void Render() override;

	void addWindows();
	void resetBoard();
};

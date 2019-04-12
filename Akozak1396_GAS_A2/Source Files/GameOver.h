/*
*  FILE          : GameOver.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are defining the GameOver class which is a child of the GameLevel class. This
*	 screen is diplayed when the user looses the game.
*/

#pragma once
#include "GameController.h"
#include "Graphics.h"
#include "MainGame.h"
#include "GameLevel.h"

class GameOver : public GameLevel
{
public:
	void Load() override;
	void Unload() override;
	void UnPause() override;
	void Render() override;
	void Update() override;
};
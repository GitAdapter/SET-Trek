/*
*  FILE          : GameLevel.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to define the functions of GameLevel and also include
*	 the library of sound files which can be played.
*/

#pragma once
#include <map>
#define _CRT_SECURE_NO_WARNINGS
#include "Graphics.h"

enum Sound { explosion, background, title, combat, gameOver, loading, confirm, close, click, error };
typedef std::map<Sound, const char*> sl;

class GameLevel
{
protected:
	static Graphics* gfx;

	sl soundLibrary = {
		{Sound::background, "Resources\\sounds\\slow-travel.wav"},
		{Sound::explosion, "Resources\\sounds\\explosion.wav" },
		{Sound::title, "Resources\\sounds\\loading.wav"},
		{Sound::gameOver, "Resources\\sounds\\scream.wav"},
		{Sound::loading, "Resources\\sounds\\loading.wav"},
		{Sound::confirm, "Resources\\sounds\\confirm.wav"},
		{Sound::close, "Resources\\sounds\\close.wav"},
		{Sound::click, "Resources\\sounds\\click.wav"},
		{Sound::error, "Resources\\sounds\\error.wav"},
		{Sound::combat, "Resources\\sounds\\ShootingStars.wav"}
	};
public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
	}

	virtual void Load() = 0;
	virtual void UnPause() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};
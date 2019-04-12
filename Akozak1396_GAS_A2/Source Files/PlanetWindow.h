/*
*  FILE          : PlanetWindow.h
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to initialize PlanetWindow class, which is a level
*	 created by posessed by each individual planet. This is a child of GameLevel so a number of functions
*	 are required by that fact. This tracks the animation playing in the center of the screen along with the
*	 current energy and science levels of the planet. 
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "GameLevel.h"


class PlanetWindow : public GameLevel
{
public:
	void Load() override;
	void Unload() override;
	void UnPause() override;
	void Render() override;
	void Update() override;

	floatPOINT *holdingDestination;

	void* planetAnimation = nullptr;

	bool visited = false;

	int getScience() { return science; }
	int getEnergy() { return energy; }

	std::wstring planetName;
	int energy = -1;
	int science = -1;
};
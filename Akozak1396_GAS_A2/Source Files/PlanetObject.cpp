/*
*  FILE          : PlanetObject.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file are used to define the functionality of the PlanetObject class,
*	 which simply sets the animation on screen to go infinitely and at a random speed.
*/

#pragma once
#include "VisibleObject.h"

PlanetObject::PlanetObject(AnimationObject* an) : AnimationObject(an)
{
	repeatTimes = -1;
	frameSpeed = rand() % 10 + 10;
}

PlanetObject::~PlanetObject()
{
}
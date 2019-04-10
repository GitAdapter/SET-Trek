#pragma once
#include "GameLevel.h"
#include "MovableObject.h"
#include "PlanetObject.h"

class Level1 : public GameLevel
{
	floatPOINT* anchor = new floatPOINT;
	floatPOINT shipPosition;

	MovableObject* background;
	
	MovableObject* playerShip;
	MovableObject* playerDetails;
	MovableObject* enemyPointer;
	
	MovableObject* enemyShip;

	std::vector<AnimationObject*> planets;
	std::vector<AnimationObject*> explosions;
	std::vector<AnimationObject*> randomEnvironment;

	AnimationObject* boxes;
	AnimationObject* shootingStar;

	std::vector<PlanetObject> currSprites;
	std::vector<PlanetObject> onScreenPlanets;
	std::list<AnimationObject> animations;

public:
	void Load() override;
	void Unload() override;
	void UnPause() override;
	void Update() override;
	void Render() override;
	void checkPlanetCollision();
	void repopSector();
	void resetBoard();
};

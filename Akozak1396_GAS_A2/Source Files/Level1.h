#pragma once
#include "GameLevel.h"
#include "MovableObject.h"
#include "AnimationObject.h"

struct Planet {
	MovableObject* obj = new MovableObject();
};

class Level1 : public GameLevel
{
	floatPOINT* anchor = new floatPOINT;
	floatPOINT shipPosition;

	MovableObject* background;

	MovableObject* basePlanet1;
	MovableObject* basePlanet2;
	MovableObject* basePlanet3;
	
	MovableObject* playerShip;
	MovableObject* playerDetails;
	MovableObject* enemyPointer;
	
	MovableObject* enemyShip;

	AnimationObject* explosion1;
	AnimationObject* shootingStar;

	std::vector<Planet> currSprites;
	std::vector<Planet> onScreenPlanets;
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

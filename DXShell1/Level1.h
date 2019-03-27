#pragma once
#include "GameLevel.h"
#include "MovableObject.h"

struct Planet {
	int x = 0;
	int y = 0;
	MovableObject* obj = new MovableObject();
};

class Level1 : public GameLevel
{
	MovableObject* background;

	MovableObject* basePlanet1;
	MovableObject* basePlanet2;
	MovableObject* basePlanet3;
	
	MovableObject* playerShip;
	MovableObject* playerDetails;
	
	MovableObject* enemyShip;

	std::list<Planet> currSprites;

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

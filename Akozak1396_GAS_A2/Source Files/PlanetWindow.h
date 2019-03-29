#pragma once
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

	bool visited = false;

	int getScience() { return science; }
	int getEnergy() { return energy; }

	std::wstring planetName;
	int energy = -1;
	int science = -1;
};
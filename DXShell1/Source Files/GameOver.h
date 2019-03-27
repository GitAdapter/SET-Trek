#pragma once
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
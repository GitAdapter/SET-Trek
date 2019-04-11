#pragma once
#include "GameLevel.h"

class Level1 : public GameLevel
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

#include "GameOver.h"
#include "Graphics.h"

void GameOver::Load()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	D2D1_SIZE_F windowSize = rt->GetSize();
	gfx->ClearScreen(0, 0, 0);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Science Gathered: %d", *gfx->Science);

	gfx->DrawScreenText(CurrEnergyString, 0, windowSize.height - 100, windowSize.width, 100, D2D1::ColorF::Gold, 56);
	
	gfx->DrawScreenText(L"GAME OVER", 0, windowSize.height / 2 - 75, windowSize.width, 125, D2D1::ColorF::White, 125);
}

void GameOver::Unload()
{

}

void GameOver::UnPause()
{

}

void GameOver::Render()
{

}

void GameOver::Update()
{

}
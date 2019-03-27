#include "PlanetWindow.h"
#include "GameController.h"

void PlanetWindow::Load()
{
	holdingDestination = gfx->destination;

	gfx->destination = new POINTF{ 0.0f, 0.0f };

	if (energy == -1)
	{
		energy = (rand() % 300) + 1;
		science = (rand() % 300) + 1;
		std::string pn = gfx->planetNames.at(rand() % gfx->planetNames.size());
		planetName = std::wstring(pn.begin(), pn.end());
	}
}

void PlanetWindow::Unload()
{

}

void PlanetWindow::Update()
{

}

void PlanetWindow::UnPause()
{

}

void PlanetWindow::Render()
{
	ID2D1RenderTarget *rt = gfx->GetRenderTarget();
	D2D1_SIZE_F size = rt->GetSize();
	gfx->DrawRect(0, 0, size.width, size.height, D2D1::ColorF::Black, true);
	gfx->DrawRect(0, 0, size.width, 75, D2D1::ColorF::GhostWhite, true);
	wchar_t welcome[100] = L"Welcome to ";
	gfx->DrawScreenText(wcscat(welcome, planetName.c_str()), 0, 0, size.width, 75, D2D1::ColorF::Black, 70);

	wchar_t ScienceString[20];
	swprintf_s(ScienceString, L"Science: %d", science);

	wchar_t EnergyString[20];
	swprintf_s(EnergyString, L"Energy: %d", energy);

	wchar_t CurrScienceString[40];
	swprintf_s(CurrScienceString, L"Total Science: %d", *gfx->Science);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Energy: %d", *gfx->Energy);

	gfx->DrawScreenText(ScienceString, 0, 100, size.width/2, 75, D2D1::ColorF::CornflowerBlue, 50);
	gfx->DrawScreenText(EnergyString, 0, 175, size.width/2, 75, D2D1::ColorF::LightGoldenrodYellow, 50);

	gfx->DrawScreenText(CurrScienceString, size.width / 2, 100, size.width / 2, 75, D2D1::ColorF::CornflowerBlue, 28);
	gfx->DrawScreenText(CurrEnergyString, size.width / 2, 175, size.width / 2, 75, D2D1::ColorF::LightGoldenrodYellow, 28);

	gfx->DrawRect(0, 300, size.width, 75, D2D1::ColorF::GhostWhite, true);
	gfx->DrawScreenText(L"Orders Capitan?", 0, 300, size.width, 75, D2D1::ColorF::Black, 36);
	
	gfx->DrawRect(0, size.height - 225, size.width, 75, D2D1::ColorF::GhostWhite, false);
	gfx->DrawRect(0, size.height - 150, size.width, 75, D2D1::ColorF::DarkGray, false);
	gfx->DrawRect(0, size.height - 75, size.width, 75, D2D1::ColorF::GhostWhite, false);
	gfx->DrawScreenText(L"1. Replenish Energy", 0, size.height - 225, size.width, 75, D2D1::ColorF::WhiteSmoke, 28);
	gfx->DrawScreenText(L"2. Gather Science", 0, size.height - 150, size.width, 75, D2D1::ColorF::WhiteSmoke, 28);
	gfx->DrawScreenText(L"3. Leave Orbit", 0, size.height - 75, size.width, 75, D2D1::ColorF::WhiteSmoke, 28);



	if (gfx->destination->y > size.height - 225)
	{
		if (gfx->destination->y < size.height - 150)
		{
			*gfx->Energy += energy;
			if (*gfx->Energy > 900) { *gfx->Energy = 900; }
			energy = 0;
		}
		else if (gfx->destination->y < size.height - 75)
		{
			*gfx->Science += science;
			science = 0;
		}
		else
		{
			gfx->destination = holdingDestination;
			GameController::ClosePopUp();
		}
	}
}
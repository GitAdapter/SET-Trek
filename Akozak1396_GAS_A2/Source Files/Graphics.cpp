#ifndef OBJECTS
#include "Graphics.h"
#endif // !OBJECTS


/***********************************************************************************
The intent of the Graphics class is to handle our DirectX calls, and to be largely responsible 
for managing the rendertarget.
******************************************************************************************/

#define UNIVERSE_WIDTH 100
#define UNIVERSE_HEIGHT UNIVERSE_WIDTH
#define CHANCE_OF_PLANET 25
#define ENEMY_DISTANCE 500

//Constructor for Graphics class
Graphics::Graphics()
{
	CreateDeviceIndependentResources(50.0f);
	factory = NULL;
	rendertarget = NULL;
	brush = NULL;

	std::ifstream inFile;
	std::string pName = "";
	inFile.open("Resources\\planetNames.txt");
	while (std::getline(inFile, pName))
	{
		if (pName.length() > 2)
		{
			planetNames.push_back(pName);
		}
	}
	inFile.close();
}

//Destructor for Graphics class
//Note that all COM objects we instantiate should be 'released' here 
//Look for comments on COM usage in the corresponding header file.

void Graphics::UpdateVisuals()
{
	canDrag = true;
	if (destination->x != -1)
	{
		*playerShip->desintation = *playerShip->location + *destination;
		playerShip->desintation->x -= shipPosition.x;
		playerShip->desintation->y -= shipPosition.y;
		destination->x = -1;
	}
	bool isMoving = !(playerShip->desintation->x == playerShip->location->x);
	*anchor = *playerShip->location;

	if (isMoving)
	{
		enemyShip->moveObject(true);
		playerShip->moveObject(false);
	}
}

void Graphics::RefreshSector()
{
	animations.clear();
	RepopulatePlanets();

	delete(playerShip->location);
	playerShip->location = new floatPOINT(shipPosition);
	playerShip->desintation = new floatPOINT;
	*playerShip->desintation = *playerShip->location;
	playerShip->angle = 0.0f;
	*anchor = *playerShip->location;

	enemyShip->location = new floatPOINT{ ENEMY_DISTANCE + anchor->x , ENEMY_DISTANCE + anchor->y };
	enemyShip->desintation = playerShip->location;
	enemyShip->speed = new floatPOINT{ 0,0 };

	int choicex = rand() % 100;
	int choicey = rand() % 100;

	if (choicex % 2 == 0) { enemyShip->location->x = -ENEMY_DISTANCE + anchor->x; }
	if (choicey % 2 == 0) { enemyShip->location->y = -ENEMY_DISTANCE + anchor->y; }

	*destination = *playerShip->location;
	enemyShip->moveObject(true);
}

void Graphics::RepopulatePlanets()
{
	D2D1_SIZE_F windowSize = GetRenderTarget()->GetSize();

	allPlanets.clear();

	for (int i = -UNIVERSE_HEIGHT; i < UNIVERSE_HEIGHT; i++)
	{
		for (int j = -UNIVERSE_WIDTH; j < UNIVERSE_WIDTH; j++)
		{
			if (rand() % CHANCE_OF_PLANET == 0)
			{
				PlanetObject p = new PlanetObject(planets[rand() % planets.size()]);
				p.location = new floatPOINT{ (windowSize.width / 10) * i + p.width / 2, (windowSize.height / 10) * j + p.height / 2 };
				p.anchorPoint = anchor;				
				allPlanets.emplace_back(p);
			}
		}
	}
}

void Graphics::PlayRandomEffect()
{
	D2D1_SIZE_F windowSize = GetRenderTarget()->GetSize();
	AnimationObject ss = new AnimationObject(randomEnvironment[rand() % randomEnvironment.size()]);
	ss.location->x = playerShip->location->x - (float)(rand() % (int)windowSize.width - (int)windowSize.width / 2);
	ss.location->y = playerShip->location->y - (float)(rand() % (int)windowSize.height - (int)windowSize.height / 2);
	ss.anchorPoint = anchor;
	animations.push_back(ss);
}

void Graphics::RenderShipScreen()
{
	D2D1_SIZE_F windowSize = GetRenderTarget()->GetSize();

	ClearScreen(0.0f, 0.0f, 0.5f);
	background->Draw();

	for (auto i = animations.begin(); i != animations.end(); i++)
	{
		(*i).Draw(shipPosition);
	}
	animations.remove_if(isComplete);

	if (!planetOffset) { planetOffset = new floatPOINT{ shipPosition.x - windowSize.width / 20, shipPosition.y - windowSize.height / 20 }; }

	for (int i = 0; i < onScreenPlanets.size(); i++)
	{
		onScreenPlanets[i].Draw(*planetOffset);
	}

	playerShip->Draw(shipPosition, false, playerShip->angle);
	enemyPointer->Draw(shipPosition, true, enemyShip->angle + 180);
	enemyShip->Draw(*enemyShip->location + shipPosition, true, enemyShip->angle + 180);

	if (*playerShip->location != oldShipPosition && (((int)playerShip->location->x % (int)(windowSize.width / 10)) == 0 || ((int)playerShip->location->y % (int)(windowSize.height / 10)) == 0))
	{
		float xMAX = playerShip->location->x + (windowSize.width);
		float xMIN = playerShip->location->x - (windowSize.width);
		float yMAX = playerShip->location->y + (windowSize.height);
		float yMIN = playerShip->location->y - (windowSize.height);

		onScreenPlanets.clear();

		for (auto i = allPlanets.begin(); i != allPlanets.end(); i++)
		{
			float x = (*i).location->x, y = (*i).location->y;
			if (x > xMIN && x < xMAX && y > yMIN && y < yMAX)
			{
				onScreenPlanets.push_back(*i);
			}
		}
	}

	wchar_t CurrScienceString[40];
	swprintf_s(CurrScienceString, L"Total Science: %d", *Science);
	//swprintf_s(CurrScienceString, L"Enemy: %0.2f, %0.2f", enemyShip->location->x, enemyShip->location->y);

	wchar_t CurrEnergyString[40];
	swprintf_s(CurrEnergyString, L"Total Energy: %d", *Energy);
	//swprintf_s(CurrEnergyString, L"Player: %0.2f, %0.2f; %0.2f", playerShip->location->x, playerShip->location->y, sqrtf(pow(playerShip->speed->x,2) + pow(playerShip->speed->y, 2)));

	DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	DrawRect(0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	DrawScreenText(CurrScienceString, 0, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::White, 24);

	DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::DarkGray, true);
	DrawRect(windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::Black, false);
	DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 30, windowSize.width / 2, 30, D2D1::ColorF::White, 24);

	//if (gfx->message != nullptr)
	//{
	//	if (gfx->message->message != 0)
	//	{
	//		swprintf_s(CurrEnergyString, L"Mesage Number: %d", gfx->message->message);
	//		gfx->DrawScreenText(CurrEnergyString, windowSize.width / 2, windowSize.height - 60, windowSize.width / 2, 30, D2D1::ColorF::White, 24);
	//	}
	//}
	oldShipPosition.x = playerShip->location->x;
	oldShipPosition.y = playerShip->location->y;
}

bool isComplete(const AnimationObject& value) 
{ 
	return value.completedAnimation; 
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();
}

//Provide some comments for each of the methods below.
//Be sure you get a sense of what is happening, and resolve any issues you have understanding these
// methods, their parameters, returns and so on.
bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect); //set the rect's right and bottom properties = the client window's size

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&rendertarget);
	if (res != S_OK) return false;
	
	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;
	return true;
}

void Graphics::LoadResources()
{
	playerShip = new MovableObject(L"Resources\\images\\PlayerShip.png", GetRenderTarget(), GetDeviceContext(), true, new floatPOINT());

	ID2D1RenderTarget* rt = GetRenderTarget();
	*anchor = *playerShip->location;

	shipPosition.x = rt->GetSize().width / 2 + 1;
	shipPosition.y = rt->GetSize().height / 2;

	background = new MovableObject(L"Resources\\images\\bg.jpg", GetRenderTarget(), GetDeviceContext(), false, anchor, { 0,0,0 }, 0.05, 0.05);
	background->location = &shipPosition;
	*background->anchorPoint = *anchor;

	planets.push_back(new AnimationObject(L"Resources\\images\\p1.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p2.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p3.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p4.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p5.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p6.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p7.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p8.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p9.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));
	planets.push_back(new AnimationObject(L"Resources\\images\\p10.png", GetRenderTarget(), GetDeviceContext(), anchor, 10, 10, 4, 4, 5));

	playerDetails = new MovableObject(L"Resources\\images\\ShipDetail.bmp", GetRenderTarget(), GetDeviceContext(), true, playerShip->anchorPoint); //This is where we can specify our file system object!
	enemyPointer = new MovableObject(L"Resources\\images\\EnemyDirection.bmp", GetRenderTarget(), GetDeviceContext(), true, playerShip->anchorPoint, { 0.0f, 1.0f, 0.0f }, 5, 5); //This is where we can specify our file system object!
	enemyShip = new MovableObject(L"Resources\\images\\EnemyShip.bmp", GetRenderTarget(), GetDeviceContext(), false, anchor, { 0.0f, 0.0f, 1.0f });
	enemyShip->desintation = playerShip->location;

	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe1.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe2.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe3.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 1, 60, 5, -1));
	globes.push_back(new AnimationObject(L"Resources\\images\\spinningGlobe4.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 1, 60, 5, -1));

	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion1.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 10, 7, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion2.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 10, 8, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion3.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 10, 9, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion4.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 10, 8, 2));
	explosions.push_back(new AnimationObject(L"Resources\\images\\explosion5.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 10, 7, 2));

	randomEnvironment.push_back(new AnimationObject(L"Resources\\images\\ShootingStar.png", GetRenderTarget(), GetDeviceContext(), anchor, 0, 0, 1, 23, 2));
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(randomEnvironment[0]);
	randomEnvironment.push_back(explosions[0]);
	randomEnvironment.push_back(explosions[1]);
	randomEnvironment.push_back(explosions[2]);
	randomEnvironment.push_back(explosions[3]);
	randomEnvironment.push_back(explosions[4]);

	boxes = new AnimationObject(L"Resources\\images\\chicken.png", GetRenderTarget(), GetDeviceContext(), anchor, 2, 2, 1, 41, 8);

	RefreshSector();
}

void Graphics::ClearScreen(float r, float g, float b) 
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	rendertarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);	
}

void Graphics::DrawScreenText(const WCHAR* string, float x, float y, float width, float height, D2D1::ColorF color, FLOAT fontSize)
{
	CreateDeviceIndependentResources(fontSize);
	brush->SetColor(color);
	rendertarget->DrawTextA(string, UINT32(wcslen(string)), m_pTextFormat, D2D1::RectF(x, y, x + width, y + height), brush);
}

void Graphics::DrawRect(float x, float y, float width, float height, D2D1::ColorF color, bool fill)
{
	brush->SetColor(color);
	D2D1_RECT_F rect = D2D1::RectF(x, y, x + width, y + height);

	if (fill) { rendertarget->FillRectangle(rect, brush); }
	else { rendertarget->DrawRectangle(rect, brush); }
}

HRESULT Graphics::CreateDeviceIndependentResources(FLOAT fontSize)
{
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;
	HRESULT hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
	);
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"", //locale
			&m_pTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return hr;
}
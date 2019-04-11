#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma warning(disable : 4244)

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <d2d1_1.h>
#include <d2d1.h>
#include <d3d11_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <d2d1effects_2.h>
#include <d2d1effecthelpers.h>
#include <dwrite.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <math.h>
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <concrt.h>
#include "VisibleObject.h"

#ifndef F_POINT
#include "floatPoint.h"
#endif // !F_POINT

#define GRAPHICS_DEFINED

bool isComplete(const AnimationObject& value);

class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory *factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget *rendertarget; //this is typically an area in our GPU memory.. like a back buffer 	
	
public:
	Graphics* getGraphics() { return this; }
	MSG *message;
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!
	IDWriteFactory *m_pDWriteFactory;
	IDWriteTextFormat *m_pTextFormat;

	floatPOINT* anchor = new floatPOINT;
	floatPOINT shipPosition;
	floatPOINT oldShipPosition;

	MovableObject* background;

	MovableObject* playerShip;
	MovableObject* playerDetails;
	MovableObject* enemyPointer;

	MovableObject* enemyShip;

	std::vector<AnimationObject*> planets;
	std::vector<AnimationObject*> explosions;
	std::vector<AnimationObject*> randomEnvironment;
	std::vector<AnimationObject*> globes;

	AnimationObject* boxes;
	AnimationObject* shootingStar;

	std::vector<PlanetObject> allPlanets;
	std::vector<PlanetObject> onScreenPlanets;
	std::list<AnimationObject> animations;

	std::vector<std::string> planetNames;

	floatPOINT *destination = new floatPOINT();
	floatPOINT* planetOffset;

	int *Energy, *Science;
	bool isDragging = false, canDrag = false;

	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	ID2D1RenderTarget* GetRenderTarget()
	{
		return rendertarget;
	}

	ID2D1DeviceContext* GetDeviceContext()
	{
		ID2D1DeviceContext *rtDC;
		rendertarget->QueryInterface(&rtDC);
		return rtDC;
	}

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }

	HRESULT CreateDeviceIndependentResources(FLOAT);
	void ClearScreen(float r, float g, float b);
	void DrawCircle(float c, float y, float radius, float r, float g, float b, float a);
	void DrawScreenText(const WCHAR* string, float x, float y, float width, float height, D2D1::ColorF color, FLOAT);
	void DrawRect(float x, float y, float width, float height, D2D1::ColorF color, bool fill);

	void UpdateVisuals();
	void RefreshSector();
	void RepopulatePlanets();
	void RenderShipScreen();
	void LoadResources();
	void PlayRandomEffect();
};

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
#include "floatPoint.h"


class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory *factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget *rendertarget; //this is typically an area in our GPU memory.. like a back buffer 	
	
public:
	MSG *message;
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!
	IDWriteFactory *m_pDWriteFactory;
	IDWriteTextFormat *m_pTextFormat;

	std::vector<std::string> planetNames;

	floatPOINT *destination = new floatPOINT();

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
	//void DrawTransformCircle(float c, float y, float radius, float r, float g, float b, float a);
	void DrawScreenText(const WCHAR* string, float x, float y, float width, float height, D2D1::ColorF color, FLOAT);
	void DrawRect(float x, float y, float width, float height, D2D1::ColorF color, bool fill);
	// Hrmmm... r, g, b, a? Where do we know these from?
};

#include <Windows.h>
#include "Graphics.h"
#include "Level1.h"
#include "GameController.h"
#include <time.h> 
#include <windowsx.h>

Graphics* graphics;

/*
Your Mission: Document the following code. What do all these components do?
*/
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam )
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }
	
//BIG CHANGE! We are not Drawing/rendering here! 
// We've moved this down to the message handling loop below

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Have you seen a 'main' for a Win32 app before? Please comment this.
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPWSTR cmd,
	int nCmdShow
)
{
	//Do a bit of reading - What is this Window Class used for? 
	// What are the major parameters below?
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW; //Alert - This is useful here... what does it do?

	RegisterClassEx(&windowclass);

	srand(time(NULL));

	RECT rect = { 0, 0, 1600, 900 };//Do these numbers look significant to you? What are they?
	AdjustWindowRectEx(&rect, WS_OVERLAPPED, false, WS_EX_OVERLAPPEDWINDOW);

	//Below is another important process to understand... what are we doing?
	//Why is this connected to rect we just defined above?
	HWND windowhandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "SET-TREK", WS_OVERLAPPEDWINDOW, 100, 100, 
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);
	if (!windowhandle) return -1;

	graphics = new Graphics();
	if (!graphics->Init(windowhandle))
	{
		delete graphics;
		return -1;
	}

	GameLevel::Init(graphics);
	ShowWindow(windowhandle, nCmdShow);	
	GameController::LoadInitialLevel(new Level1());

	graphics->LoadResources();
#pragma region GameLoop
	//Below, we have essentially an infinite loop that will keep the window running and will dispatch/show messages
	//As many people will tell you, most Windows you see are just infinite loops waiting for some kind of work-flow or 
	//system-based interuption.

	//Note - Our message handling has changed from the first demo code.
	//Here, we use a PeekMessage to avoid locking the graphics/windoProc
	//when a message has to be dispatched.
	MSG message;
	message.message = WM_NULL; //Do not have this set to WM_QUIT, which has a specific context
	graphics->message = &message;
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			//This allows us to send a message to the WindowProc IF there is one
			DispatchMessage(&message);
		else
		{	
			if (!graphics->canDrag)
			{
				graphics->isDragging = false;
			}

			if (message.message == WM_LBUTTONUP)
			{
				graphics->isDragging = false;
			}
			else if (message.message == WM_LBUTTONDOWN || graphics->isDragging * graphics->canDrag)
			{
				graphics->destination->x = GET_X_LPARAM(message.lParam);
				graphics->destination->y = GET_Y_LPARAM(message.lParam);
				graphics->isDragging = true;
			}					
			//Update Routine... we've moved the code for handling updates to GameController
			GameController::Update();

			//Render Routine... This is very modular. GameController now handles the rendering
			graphics->BeginDraw();
			//graphics->background->Draw();
			GameController::Render();
			//graphics->DrawCircle(10, 10, 100, 10, 10, 10, 10);
			graphics->EndDraw();
			message.message = WM_NULL;
		}
	}

#pragma endregion
	delete graphics;
	return 0;
}
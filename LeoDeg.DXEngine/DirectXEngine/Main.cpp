#include <Windows.h>
#include "RenderWindow.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DirectXTK.lib")


int APIENTRY wWinMain (_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCMdLine,
	_In_ int nCmdShow)
{

	DXEngine::RenderWindow renderWindow;
	renderWindow.Initialize (hInstance, "TestWindow", "TestWindowClass", 500, 500);

	while (renderWindow.ProcessMessages () == true)
	{

	}

	return 0;
}
#include <Windows.h>
#include "Engine.h"



int APIENTRY wWinMain (_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCMdLine,
	_In_ int nCmdShow)
{
	DXEngine::Engine engine;
	if (engine.Initialize (hInstance, "TestWindow", "TestWindowClass", 500, 500))
	{
		while (engine.ProcessMessages () == true)
		{
			engine.Update ();
			engine.RenderFrame ();
		}
	}

	return 0;
}
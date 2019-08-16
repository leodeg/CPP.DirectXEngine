#include <Windows.h>
#include "Engine.h"


int APIENTRY wWinMain (_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCMdLine,
	_In_ int nCmdShow)
{
	HRESULT hResult = CoInitialize (NULL);
	if (FAILED (hResult))
	{
		DXEngine::ErrorLogger::Log (NULL, "Failed to call CoInitialize.");
		return -1;
	}

	DXEngine::Engine engine;
	if (engine.Initialize (hInstance, "LeoDeg.DXEngine", "LeoDeg.DXEngine.Window.Class", 1600, 900))
	{
		while (engine.ProcessMessages () == true)
		{
			engine.Update ();
			engine.RenderFrame ();
		}
	}

	return 0;
}
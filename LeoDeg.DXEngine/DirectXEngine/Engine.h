#pragma once
#include "WindowContainer.h"
#include "ErrorLogger.h"

namespace DXEngine
{
	class Engine : WindowContainer
	{
	public:
		bool Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
		bool ProcessMessages ();
		void RenderFrame ();
		void Update ();

	protected:

	private:
	};
}

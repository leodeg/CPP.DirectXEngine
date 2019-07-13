#pragma once
#include "WindowContainer.h"

namespace DXEngine
{
	class Engine : WindowContainer
	{
	public:
		bool Initialize (HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
		bool ProcessMessages ();
		void Update ();

	protected:

	private:
	};
}
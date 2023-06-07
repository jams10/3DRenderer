#pragma once

#include "Utility/CustomMacros.h"

#include <Windows/WindowsHeaders.h>

namespace NS
{
	class Mouse
	{
	public:
		void Move(WPARAM buttonState, int x, int y, int screenWidth, int screenHeight);
		float GetPositionX() { return posX; }
		float GetPositionY() { return posY; }

	private:
		// ndc 좌표계 마우스 위치.
		float posX; 
		float posY;
	};
}


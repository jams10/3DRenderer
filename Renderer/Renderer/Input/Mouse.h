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
		// ndc ��ǥ�� ���콺 ��ġ.
		float posX; 
		float posY;
	};
}


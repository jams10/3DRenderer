#include "RenderApp.h"

namespace NS
{
	RenderApp::RenderApp(int screenWidth, int screenHeight)
		: AppBase(screenWidth, screenHeight)
	{}

	bool RenderApp::Initialize()
	{
		if (AppBase::Initialize() == false)
			return false;

		return true;
	}

	void RenderApp::Update(float dt)
	{
	}

	void RenderApp::Render()
	{
	}
}
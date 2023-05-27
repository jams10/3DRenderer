
#include "RenderApp.h"

#include "Scene/SCDrawTriangle.h"
#include "Scene/SCDrawTexturedCube.h"

namespace NS
{
	RenderApp::RenderApp(int screenWidth, int screenHeight)
		: AppBase(screenWidth, screenHeight)
	{
		m_pGraphics = nullptr;
		m_pScene = nullptr;
	}

	bool RenderApp::Initialize()
	{
		if (AppBase::Initialize() == false)
			return false;

		m_pScene = new SCDrawTexturedCube();
		m_pScene->Initialize(m_pGraphics);

		return true;
	}

	void RenderApp::UpdateGUI()
	{
		m_pScene->UpdateGUI();
	}

	void RenderApp::Update(float dt)
	{
		AppBase::Update(dt);

		m_pScene->Update(dt);
	}

	void RenderApp::Render()
	{
		m_pScene->Render();
	}
}
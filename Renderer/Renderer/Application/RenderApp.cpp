
#include "RenderApp.h"

#include <imgui.h>
#include <iostream>

#include "Scene/Scenes.h"
#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

#include "Input/Keyboard.h"

namespace NS
{
	RenderApp::RenderApp(int screenWidth, int screenHeight)
		: AppBase(screenWidth, screenHeight)
	{
		m_pGraphics = nullptr;
		m_pScene = nullptr;
		m_selectedSceneIndex = -1;
		m_currentSceneIndex = -2;
	}

	bool RenderApp::Initialize()
	{
		if (AppBase::Initialize() == false)
			return false;

		m_camera.SetAspectRatio(m_pGraphics->GetD3D11()->GetAspectRatio());

		return true;
	}

	void RenderApp::UpdateGUI()
	{
		ShowTitleMenuWindow(); 	 // 메인 메뉴 창.
		ShowSystemInfoWindow();  // 시스템 정보 창.
		ShowSceneSelectWindow(); // 씬 정보 창.
		
		if(m_pScene)
			m_pScene->UpdateGUI();   // 씬 UI 창.
	}

	void RenderApp::Update(float dt)
	{
		AppBase::Update(dt);

		if (m_pScene)
			m_pScene->Update(dt);

		m_camera.SetAspectRatio(m_pGraphics->GetD3D11()->GetAspectRatio());

		MoveCamera(dt);
	}

	void RenderApp::Render()
	{
		if (m_pScene)
			m_pScene->Render();
	}

	void RenderApp::MoveCamera(float dt)
	{
		if (m_keyboard.m_keyPressed['W'])
		{
			m_camera.MoveForward(dt);
		}
		if (m_keyboard.m_keyPressed['A'])
		{
			m_camera.MoveRight(-dt);
		}
		if (m_keyboard.m_keyPressed['S'])
		{
			m_camera.MoveForward(-dt);
		}
		if (m_keyboard.m_keyPressed['D'])
		{
			m_camera.MoveRight(dt);
		}
		if (m_keyboard.m_keyPressed['T'])
		{
			m_camera.MoveUpward(dt);
		}
		if (m_keyboard.m_keyPressed['R'])
		{
			m_camera.MoveUpward(-dt);
		}

		if (m_keyboard.m_keyToggle['F'])
		{
			m_camera.ToggleFlyingCam();
			m_keyboard.m_keyToggle['F'] = false;
		}

		m_camera.RotateCameraWithMouse(m_mouse.GetPositionX(), m_mouse.GetPositionY());
		m_camera.UpdateViewDirection();
	}

#pragma region UI
	void RenderApp::ShowTitleMenuWindow()
	{
		ImGui::BeginMainMenuBar();
		{
			if (ImGui::BeginMenu(WideToMultiU8(L"창").c_str()))
			{
				if (ImGui::MenuItem(WideToMultiU8(L"시스템 정보").c_str()))
					m_openSystemInfoWindow = true;

				if (ImGui::MenuItem(WideToMultiU8(L"씬").c_str()))
					m_openSceneSelectWindow = true;
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}

	void RenderApp::ShowSystemInfoWindow()
	{
		// 시스템 정보 창.
		if (m_openSystemInfoWindow)
		{
			ImGui::Begin(WideToMultiU8(L"시스템 정보").c_str(), &m_openSystemInfoWindow);
			{
				// ImGui가 측정해주는 Framerate 출력.
				ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End();
		}
	}

	void RenderApp::ShowSceneSelectWindow()
	{
		bool selected = false;
		if (m_openSceneSelectWindow)
		{
			ImGui::Begin(WideToMultiU8(L"씬 선택").c_str(), &m_openSceneSelectWindow);
			{
				if (ImGui::Button(WideToMultiU8(L"삼각형 그리기").c_str()))
				{
					m_selectedSceneIndex = 0;
					selected = true;
				}
				if (ImGui::Button(WideToMultiU8(L"텍스쳐 큐브 그리기").c_str()))
				{
					m_selectedSceneIndex = 1;
					selected = true;
				}
			}
			ImGui::End();
		}

		if (selected)
		{
			if (m_currentSceneIndex == m_selectedSceneIndex) return;
			if (m_pScene != nullptr)
			{
				delete m_pScene;
				m_pScene = nullptr;
			}
			m_currentSceneIndex = m_selectedSceneIndex;
			m_openSceneSelectWindow = false;

			SceneType type = (SceneType)(m_currentSceneIndex);
			switch (type)
			{
			case SceneType::DRAW_TRIANGLE:
				m_pScene = new SCDrawTriangle;
				m_pScene->Initialize(m_pGraphics, &m_camera);
				break;
			case SceneType::DRAW_TEXTUREDCUBE:
				m_pScene = new SCDrawTexturedCube;
				m_pScene->Initialize(m_pGraphics, &m_camera);
				break;
			}
		}
	}

#pragma endregion
}

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
		ShowTitleMenuWindow(); 	 // ���� �޴� â.
		ShowSystemInfoWindow();  // �ý��� ���� â.
		ShowSceneSelectWindow(); // �� ���� â.
		
		if(m_pScene)
			m_pScene->UpdateGUI();   // �� UI â.
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
			if (ImGui::BeginMenu(WideToMultiU8(L"â").c_str()))
			{
				if (ImGui::MenuItem(WideToMultiU8(L"�ý��� ����").c_str()))
					m_openSystemInfoWindow = true;

				if (ImGui::MenuItem(WideToMultiU8(L"��").c_str()))
					m_openSceneSelectWindow = true;
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}

	void RenderApp::ShowSystemInfoWindow()
	{
		// �ý��� ���� â.
		if (m_openSystemInfoWindow)
		{
			ImGui::Begin(WideToMultiU8(L"�ý��� ����").c_str(), &m_openSystemInfoWindow);
			{
				// ImGui�� �������ִ� Framerate ���.
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
			ImGui::Begin(WideToMultiU8(L"�� ����").c_str(), &m_openSceneSelectWindow);
			{
				if (ImGui::Button(WideToMultiU8(L"�ﰢ�� �׸���").c_str()))
				{
					m_selectedSceneIndex = 0;
					selected = true;
				}
				if (ImGui::Button(WideToMultiU8(L"�ؽ��� ť�� �׸���").c_str()))
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
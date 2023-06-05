
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

		m_keyboard.EnableAutorepeat();
		m_Camera.SetAspectRatio(m_pGraphics->GetD3D11()->GetAspectRatio());

		return true;
	}

	void RenderApp::UpdateGUI()
	{
		ShowTitleMenuWindow(); 	 // ���� �޴� â.
		ShowSystemInfoWindow();  // �ý��� ���� â.
		ShowSceneSelectWindow(); // �� ���� â.
		
		m_Camera.UpdateGUI();

		if(m_pScene)
			m_pScene->UpdateGUI();   // �� UI â.

	}

	void RenderApp::Update(float dt)
	{
		AppBase::Update(dt);

		if (m_pScene)
			m_pScene->Update(dt);

		m_Camera.SetAspectRatio(m_pGraphics->GetD3D11()->GetAspectRatio());

		while (const auto evt = m_keyboard.ReadKey())
		{
			if (!evt->IsPress()) // Ű�� ������ �ʾҴٸ� �н�.
			{
				continue;
			}

			MoveCamera(dt);
		}
	}

	void RenderApp::Render()
	{
		if (m_pScene)
			m_pScene->Render();
	}

	void RenderApp::MoveCamera(float dt)
	{
		if (m_keyboard.KeyIsPressed('W'))
		{
			m_Camera.Translate(0.0f, 0.0f, 1.0f, dt);
		}
		if (m_keyboard.KeyIsPressed('A'))
		{
			m_Camera.Translate(-1.0f, 0.0f, 0.0f, dt);
		}
		if (m_keyboard.KeyIsPressed('S'))
		{
			m_Camera.Translate(0.0f, 0.0f, -1.0f, dt);
		}
		if (m_keyboard.KeyIsPressed('D'))
		{
			m_Camera.Translate(1.0f, 0.0f, 0.0f, dt);
		}
		if (m_keyboard.KeyIsPressed('R'))
		{
			m_Camera.Translate(0.0f, 1.0f, 0.0f, dt);
		}
		if (m_keyboard.KeyIsPressed('F'))
		{
			m_Camera.Translate(0.0f, -1.0f, 0.0f, dt);
		}

		m_Camera.UpdateViewDirection();
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
				m_pScene->Initialize(m_pGraphics, &m_Camera);
				break;
			case SceneType::DRAW_TEXTUREDCUBE:
				m_pScene = new SCDrawTexturedCube;
				m_pScene->Initialize(m_pGraphics, &m_Camera);
				break;
			}
		}
	}

#pragma endregion
}
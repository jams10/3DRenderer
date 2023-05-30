
#include "RenderApp.h"

#include <imgui.h>

#include "Scene/Scenes.h"
#include "Utility/StringEncode.h"

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
	}

	void RenderApp::Render()
	{
		if (m_pScene)
			m_pScene->Render();
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
				m_pScene->Initialize(m_pGraphics);
				break;
			case SceneType::DRAW_TEXTUREDCUBE:
				m_pScene = new SCDrawTexturedCube;
				m_pScene->Initialize(m_pGraphics);
				break;
			}
		}
	}

#pragma endregion
}
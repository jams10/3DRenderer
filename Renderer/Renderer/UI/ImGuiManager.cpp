#include "ImGuiManager.h"

#include <imgui.h>
#include <iostream>

namespace NS
{
	using std::cout;

	ImGuiManager::ImGuiManager()
	{
		IMGUI_CHECKVERSION();     // ���� üũ
		ImGui::CreateContext();   // Imgui ���̺귯���� ������ �۾��� �ϱ� ���� ���ؽ�Ʈ ����.

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		// �ѱ� ��Ʈ �߰�.
		io.Fonts->AddFontFromFileTTF("..\\Resources\\Fonts\\SUITE-SemiBold.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());

		ImGui::StyleColorsDark(); // ui �׸��� ���������� ��������.
	}

	ImGuiManager::~ImGuiManager()
	{
		ImGui::DestroyContext();  // ������ ���ؽ�Ʈ ����.
	}
}
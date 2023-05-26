#include "ImGuiManager.h"

#include <imgui.h>
#include <iostream>

namespace NS
{
	using std::cout;

	ImGuiManager::ImGuiManager()
	{
		IMGUI_CHECKVERSION();     // 버전 체크
		ImGui::CreateContext();   // Imgui 라이브러리를 가지고 작업을 하기 위한 컨텍스트 생성.

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		// 한글 폰트 추가.
		io.Fonts->AddFontFromFileTTF("..\\Resources\\Fonts\\SUITE-SemiBold.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());

		ImGui::StyleColorsDark(); // ui 테마를 검정색으로 설정해줌.
	}

	ImGuiManager::~ImGuiManager()
	{
		ImGui::DestroyContext();  // 생성한 컨텍스트 제거.
	}
}
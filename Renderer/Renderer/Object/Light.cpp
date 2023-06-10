
#include "Light.h"

#include <imgui.h>

namespace NS
{
    void Light::Update()
    {

    }

    void Light::UpdateGUI()
    {
		ImGui::Begin("Light");

		ImGui::Text("Transform");
		ImGui::SliderFloat3("Translation", &m_lightConstant.position.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation(Rad)", &m_rotation.x, -3.14f, 3.14f);

		ImGui::Text("Lighting Options");
		ImGui::SliderFloat("Intensity", &m_intensity, 0.f, 2.0f);
		
		if (m_type == ELightType::Point || m_type == ELightType::Spot)
		{
			ImGui::SliderFloat("FallOffStart", &m_lightConstant.fallOffStart, 0.f, 5.0f);
			ImGui::SliderFloat("FallOffEnd", &m_lightConstant.fallOffStart, 0.f, 10.0f);
		}

		ImGui::ColorEdit3("Color", &m_lightConstant.color.x);

		ImGui::End();
    }
}

#include "Light.h"

#include <imgui.h>
#include <string>
#include <sstream>

#include "LightModel.h"

namespace NS
{
	void Light::Update(float dt, LightModel* pModel)
    {
		if (bTurnOn == true) m_lightConstant.turnOn = true;
		else
		{
			m_lightConstant.turnOn = false;
			return;
		}

		m_lightConstant.intensity = m_intensity * Vector3(1.f);
		m_lightConstant.direction = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationX(-this->m_pitch));
		m_lightConstant.direction = Vector3::Transform(m_lightConstant.direction, Matrix::CreateRotationY(this->m_yaw));
		m_lightConstant.type = m_type;

		// 광원을 시각적으로 나타낼 3D 메쉬의 위치와 색상을 광원과 똑같이 만들어줌.
		pModel->m_position = m_lightConstant.position;
		pModel->m_materialConstantData.color = m_lightConstant.color;
    }

    void Light::UpdateGUI()
    {
		ImGui::Begin("Light");

		ImGui::PushID(m_id);

		std::string s = "Turn On ";
		std::string type = "";
		if (m_type == 0) type = "Directional Light";
		if (m_type == 1) type = "Point Light";
		if (m_type == 2) type = "Spot Light";
		std::stringstream ss;
		ss << s << type << "(" << m_id << ")";

		ImGui::Checkbox(ss.str().c_str(), &bTurnOn);
		ImGui::Spacing();

		if(bTurnOn)
		{
			const char* items[] = { "Directional", "Point", "Spot"};
			ImGui::Combo("Type", &m_type, items, IM_ARRAYSIZE(items));

			ImGui::Text("Transform");
			ImGui::SliderFloat3("Translation", &m_lightConstant.position.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Pitch", &m_pitch, 0.995f * -90.0f, 0.995f * 90.0f);
			ImGui::SliderFloat("Yaw", &m_yaw, -180.0f, 180.0f);

			ImGui::Text("Lighting Options");
			ImGui::SliderFloat("Intensity", &m_intensity, 0.f, 2.0f);

			if (m_type == 1 || m_type == 2)
			{
				ImGui::SliderFloat("FallOffStart", &m_lightConstant.fallOffStart, 0.f, 5.0f);
				ImGui::SliderFloat("FallOffEnd", &m_lightConstant.fallOffEnd, 0.f, 10.0f);
			}
			if (m_type == 2)
			{
				ImGui::SliderFloat("SpotPower", &m_lightConstant.spotPower, 1.f, 512.0f);
			}

			ImGui::ColorEdit3("Color", &m_lightConstant.color.x);
		}

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PopID();

		ImGui::End();
    }

	void Light::SetID(int id)
	{
		m_id = id;
	}
}
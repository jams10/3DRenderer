#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

#include "Model/ConstantData.h"

namespace NS
{
	class LightModel;
	using DirectX::SimpleMath::Vector3;

	class Light
	{
	public:
		void Update(float dt, LightModel* pModel);
		void UpdateGUI();
		void SetID(int id);

		LightConstant m_lightConstant;
		float m_intensity = 1.0f;
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
		int m_type = 0;
		int m_id = 0;
		bool bTurnOn = false;
	};
}



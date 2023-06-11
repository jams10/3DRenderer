#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

#include "Model/ConstantData.h"

namespace NS
{
	using DirectX::SimpleMath::Vector3;

	class Light
	{
	public:	
		void Update();
		void UpdateGUI();
		void SetID(int id);

		LightConstant m_lightConstant;
		float m_intensity;
		float m_yaw;
		float m_pitch;
		int m_type = 0;
		bool bTurnOn = false;
		int m_id = 0;
	};
}



#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

#include "Model/ConstantData.h"

namespace NS
{
	using DirectX::SimpleMath::Vector3;

	enum ELightType
	{
		Directional,
		Point,
		Spot,
	};

	class Light
	{
	public:
		void Update();
		void UpdateGUI();

		LightConstant m_lightConstant;
		float m_intensity;
		Vector3 m_rotation;
		ELightType m_type;
	};
}



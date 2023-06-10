#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

namespace NS
{
	using DirectX::SimpleMath::Vector3;

	struct LightConstant
	{
		Vector3 intensity = Vector3(1.0f);
		float fallOffStart = 0.0f;
		Vector3 direction = Vector3(0.0f, 0.0f, 1.0f);
		float fallOffEnd = 10.0f;
		Vector3 position = Vector3(0.0f, 2.0f, 0.0f);
		float spotPower = 1.0f;
	};

	class Light
	{
	public:
		void UpdateGUI();

		LightConstant m_lightConstant;
		Vector3 m_rotation;
	};
}



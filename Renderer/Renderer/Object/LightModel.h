#pragma once

#include "Utility/CustomMacros.h"
#include "Model/Model.h"

namespace NS
{
	class LightModel : public Model
	{
	public:
		virtual void Update(float dt, GraphicsProcessor* const pGraphics) override;
		virtual void UpdateGUI() override;
	};
}



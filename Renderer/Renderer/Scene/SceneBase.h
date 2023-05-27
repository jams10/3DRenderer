#pragma once

#include <vector>

#include "Utility/CustomMacros.h"

#include "Model/MeshForCPU.h"
#include "Model/MeshForGPU.h"
#include "Model/ConstantData.h"

namespace NS
{
	class Graphics;
	using std::vector;

	class SceneBase
	{
	public:
		virtual void Initialize(Graphics* pGraphics);
		virtual void UpdateGUI() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	protected:
		Graphics* m_pGraphics;
	};
}



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
		virtual void Update(float dt);
		virtual void Render();

		void MakeTriangle();

	private:
		MeshForCPU m_triangleMeshForCPU;
		MeshForGPU m_triangleMeshForGPU;
		ConstantData_MVP m_triangleConstantData;
		Graphics* m_pGraphics;
	};
}



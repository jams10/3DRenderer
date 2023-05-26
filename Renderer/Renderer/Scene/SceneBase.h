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
		virtual void UpdateGUI();
		virtual void Update(float dt);
		virtual void Render();

		void MakeTriangle();
		void MakeCube();

	private:
		MeshForCPU m_cubeMeshForCPU;
		MeshForGPU m_cubeMeshForGPU;
		ConstantData_MVP m_cubeConstantData;
		Graphics* m_pGraphics;

		bool bUsePerspective = true;
		Vector3 m_translation = Vector3{ 0.0f, 0.0f, 1.0f };
		Vector3 m_rotation;
		Vector3 m_scale = Vector3{ 1.0f, 1.0f, 1.0f };
	};
}



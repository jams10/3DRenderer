#pragma once

#include "SceneBase.h"

namespace NS
{
	class SCDrawTexturedCube : public SceneBase
	{
	public:
		virtual void Initialize(Graphics* pGraphics) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	private:
		void MakeCube(float scale);

		MeshForCPUWithBasicVertex m_meshForCPU;
		MeshForGPU m_meshForGPU;
		ConstantDataMVPInvTranspose m_constantData;

		Vector3 m_translation = Vector3{ 0.0f, 0.0f, 2.0f };
		Vector3 m_rotation;
		Vector3 m_scale = Vector3{ 1.0f, 1.0f, 1.0f };

		bool m_bUsePerspectiveProjection = true;
		bool m_bUseWireFrame = false;
	};

}


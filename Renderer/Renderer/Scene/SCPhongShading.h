#pragma once

#include "SceneBase.h"

#include "Model/Model.h"

namespace NS
{
	class SCPhongShading : public SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics, Camera* pCamera) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
		virtual ~SCPhongShading();

	private:
		MeshForCPU MakeCube(float scale);

		Model m_cubeModel;

		bool m_bUseWireFrame = false;
	};
}


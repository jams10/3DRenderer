#pragma once

#include "SceneBase.h"

#include "Model/Model.h"
#include "Object/SkyBox.h"

namespace NS
{
	class SCEnviromentMapping : public SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics, Camera* pCamera) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
		virtual ~SCEnviromentMapping();

	private:
		Model m_model;
		SkyBox m_skyBox;
	};
}



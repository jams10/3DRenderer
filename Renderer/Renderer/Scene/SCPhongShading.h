#pragma once

#include "SceneBase.h"

#include "Model/Model.h"
#include "Object/Light.h"
#include "Object/SkyBox.h"

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
		Model m_model;
		SkyBox m_skyBox;
	};
}


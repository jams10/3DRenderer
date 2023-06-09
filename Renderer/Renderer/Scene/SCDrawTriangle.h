#pragma once

#include "SceneBase.h"

/*
*	<SCDrawTriangle>
*	기본적인 삼각형을 그리는 씬.
*	정점의 경우 위치와 색상 값을 지니도록 하여 래스터라이져를 거쳐서 보간되는 정점의 정보를 정점의 색상의 변화를 통해 눈으로 확인할 수 있음.
*/
namespace NS
{
	class SCDrawTriangle : public SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics, Camera* pCamera) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
		virtual ~SCDrawTriangle();

	private:
		Model m_triangleModel;
	};

}


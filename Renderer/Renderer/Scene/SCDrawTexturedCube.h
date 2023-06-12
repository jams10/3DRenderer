#pragma once

#include "SceneBase.h"

#include "Model/Model.h"

/*
*	<SCDrawTexturedCube>
*	기본적인 정육면체를 그리는 씬.
*	정점에 텍스쳐 좌표를 추가하고, 이미지 파일을 로드하여 파이프라인에서 사용할 텍스쳐 자원을 생성, 정점의 텍스쳐 좌표를 통해 모델에 텍스쳐를 입혀줌.
*/
namespace NS
{
	class SCDrawTexturedCube : public SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics, Camera* pCamera) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
		virtual ~SCDrawTexturedCube();

	private:
		Model m_cubeModel;
	};

}


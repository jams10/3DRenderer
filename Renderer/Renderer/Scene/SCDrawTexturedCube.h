#pragma once

#include "SceneBase.h"

#include "Model/Model.h"

/*
*	<SCDrawTexturedCube>
*	�⺻���� ������ü�� �׸��� ��.
*	������ �ؽ��� ��ǥ�� �߰��ϰ�, �̹��� ������ �ε��Ͽ� ���������ο��� ����� �ؽ��� �ڿ��� ����, ������ �ؽ��� ��ǥ�� ���� �𵨿� �ؽ��ĸ� ������.
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


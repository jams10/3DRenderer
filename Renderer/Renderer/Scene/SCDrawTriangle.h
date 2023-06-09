#pragma once

#include "SceneBase.h"

#include "Model/Model.h"

/*
*	<SCDrawTriangle>
*	�⺻���� �ﰢ���� �׸��� ��.
*	������ ��� ��ġ�� ���� ���� ���ϵ��� �Ͽ� �����Ͷ������� ���ļ� �����Ǵ� ������ ������ ������ ������ ��ȭ�� ���� ������ Ȯ���� �� ����.
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
		MeshForCPU MakeTriangle();

		Model m_triangleModel;
	};

}


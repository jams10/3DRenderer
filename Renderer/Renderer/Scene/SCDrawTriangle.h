#pragma once

#include "SceneBase.h"

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
		Model m_triangleModel;
	};

}


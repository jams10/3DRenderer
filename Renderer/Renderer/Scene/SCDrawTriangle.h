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
		virtual void Initialize(Graphics* pGraphics) override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
		virtual ~SCDrawTriangle();

	private:
		void MakeTriangle();

		MeshForCPUWithColorVertex m_meshForCPU;
		MeshForGPU m_meshForGPU;
		ConstantDataMVP m_constantData;

		Vector3 m_translation = Vector3{ 0.0f, 0.0f, 2.0f };
		Vector3 m_rotation;
		Vector3 m_scale = Vector3{ 1.0f, 1.0f, 1.0f };
	};

}


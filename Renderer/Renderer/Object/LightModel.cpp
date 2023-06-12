#include "LightModel.h"

namespace NS
{
	void LightModel::Update(float dt, GraphicsProcessor* const pGraphics)
	{
		// ���� ��ȯ ���. SRT ����.
		m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_position);

		// �� Ʈ������ ��� ���� ������Ʈ.
		UpdateModelTransformConstantBuffer(pGraphics);

		// �� ��Ƽ���� ��� ���� ������Ʈ
		UpdateModelMaterialConstantBuffer(pGraphics);
	}

	void LightModel::UpdateGUI()
	{
		// ��� �Ӽ��� Light Ŭ������ ���ǵ� ������ ���󰡱� ������ ���� �������� ����.
	}
}
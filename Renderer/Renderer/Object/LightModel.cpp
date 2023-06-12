#include "LightModel.h"

namespace NS
{
	void LightModel::Update(float dt, GraphicsProcessor* const pGraphics)
	{
		// 월드 변환 행렬. SRT 순서.
		m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_position);

		// 모델 트랜스폼 상수 버퍼 업데이트.
		UpdateModelTransformConstantBuffer(pGraphics);

		// 모델 머티리얼 상수 버퍼 업데이트
		UpdateModelMaterialConstantBuffer(pGraphics);
	}

	void LightModel::UpdateGUI()
	{
		// 모든 속성이 Light 클래스에 정의된 값들을 따라가기 때문에 따로 설정하지 않음.
	}
}
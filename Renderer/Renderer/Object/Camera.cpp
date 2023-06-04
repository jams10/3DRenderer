#include "Camera.h"

#include <imgui.h>

namespace NS
{
	Camera::Camera()
	{
		UpdateViewDirection();
	}

	Matrix Camera::GetViewMatrixRow() const
	{
		// view 변환 행렬의 목적은 물체들의 좌표계를 우리가 물체들을 바라보는 카메라, 시점을 기준 공간 좌표계로 변환하는 것.
		// 직관적으로 생각하면 카메라가 왼쪽으로 이동하면, 물체들은 오른쪽으로 이동해야 함. (반대 방향으로 translation)
		// 회전 또한 카메라가 왼쪽으로 회전하면, 물체들은 오른쪽으로 회전해야 함. (반대 방향으로 rotation)
		// pitch의 경우 시점을 아래로 내리면 값이 증가함. 
		// 그러면 시점을 위로 올리면 -pitch 값이 됨. 여기서 우리가 시점을 위로 올렸을 때 물체들은 아래로 내려가도록 -1을 또 곱하면 pitch가 됨.
		return  Matrix::CreateTranslation(-this->m_position) *
				Matrix::CreateRotationY(-this->m_yaw) *
				Matrix::CreateRotationX(this->m_pitch);
	}

	Matrix Camera::GetProjectionMatrixRow() const
	{
		return m_usePerspectiveProjection
			? DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_projFovAngleY),
				m_aspectRatio, m_nearZ, m_farZ)
			: DirectX::XMMatrixOrthographicOffCenterLH(-m_aspectRatio, m_aspectRatio, -1.0f,
				1.0f, m_nearZ, m_farZ);
	}

	Vector3 Camera::GetCameraPosition() const
	{
		return m_position;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
	}

	void Camera::UpdateViewDirection()
	{
		// 현재 사용하고 있는 좌표계는 z-forward 왼손 좌표계.
		// 카메라 초기 방향은 z축 양의 방향을 바라 보도록 했기 때문에 yaw 회전에 따라 달라지는 카메라가 보는 방향을 다시 계산함.
		m_viewDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(this->m_yaw));
		// 카메라 시점 벡터와 up 벡터를 외적하여 오른쪽 방향 벡터를 얻어줌. view 변환 행렬 생성 시 필요.
		m_rightDir = m_upDir.Cross(m_viewDir);
	}

	void Camera::UpdateGUI()
	{
		ImGui::Begin("Camera");

		ImGui::SliderFloat3("Translation", &m_position.x, -100.0f, 100.0f);
		// Pitch 값의 경우, -90, 90도로 회전해버리면 우리가 바라보는 방향 forward 벡터와 up 벡터의 방향이 일치되어 이상하게 회전됨.
		// 이를 방지하기 위해 완전히 -90, 90도가 되지 않도록 1보다 약간 작은 값을 곱해줌.
		ImGui::SliderFloat("Pitch", &m_pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderFloat("Yaw", &m_yaw, -180.0f, 180.0f);
		ImGui::Checkbox("Use Perspective", &m_usePerspectiveProjection);

		ImGui::End();
	}

}
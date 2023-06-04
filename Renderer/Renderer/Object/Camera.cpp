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
		// view ��ȯ ����� ������ ��ü���� ��ǥ�踦 �츮�� ��ü���� �ٶ󺸴� ī�޶�, ������ ���� ���� ��ǥ��� ��ȯ�ϴ� ��.
		// ���������� �����ϸ� ī�޶� �������� �̵��ϸ�, ��ü���� ���������� �̵��ؾ� ��. (�ݴ� �������� translation)
		// ȸ�� ���� ī�޶� �������� ȸ���ϸ�, ��ü���� ���������� ȸ���ؾ� ��. (�ݴ� �������� rotation)
		// pitch�� ��� ������ �Ʒ��� ������ ���� ������. 
		// �׷��� ������ ���� �ø��� -pitch ���� ��. ���⼭ �츮�� ������ ���� �÷��� �� ��ü���� �Ʒ��� ���������� -1�� �� ���ϸ� pitch�� ��.
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
		// ���� ����ϰ� �ִ� ��ǥ��� z-forward �޼� ��ǥ��.
		// ī�޶� �ʱ� ������ z�� ���� ������ �ٶ� ������ �߱� ������ yaw ȸ���� ���� �޶����� ī�޶� ���� ������ �ٽ� �����.
		m_viewDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(this->m_yaw));
		// ī�޶� ���� ���Ϳ� up ���͸� �����Ͽ� ������ ���� ���͸� �����. view ��ȯ ��� ���� �� �ʿ�.
		m_rightDir = m_upDir.Cross(m_viewDir);
	}

	void Camera::UpdateGUI()
	{
		ImGui::Begin("Camera");

		ImGui::SliderFloat3("Translation", &m_position.x, -100.0f, 100.0f);
		// Pitch ���� ���, -90, 90���� ȸ���ع����� �츮�� �ٶ󺸴� ���� forward ���Ϳ� up ������ ������ ��ġ�Ǿ� �̻��ϰ� ȸ����.
		// �̸� �����ϱ� ���� ������ -90, 90���� ���� �ʵ��� 1���� �ణ ���� ���� ������.
		ImGui::SliderFloat("Pitch", &m_pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderFloat("Yaw", &m_yaw, -180.0f, 180.0f);
		ImGui::Checkbox("Use Perspective", &m_usePerspectiveProjection);

		ImGui::End();
	}

}
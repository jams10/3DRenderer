#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

namespace NS
{
	using DirectX::SimpleMath::Matrix;
	using DirectX::SimpleMath::Vector3;

	class Camera
	{
	public:
		Camera();

		Matrix GetViewMatrixRow() const;
		Matrix GetProjectionMatrixRow() const;
		Vector3 GetCameraPosition() const;

		void SetAspectRatio(float aspectRatio);
		void RotateCameraWithMouse(float mouseNdcX, float mouseNdcY);
		void UpdateViewDirection();
		void MoveForward(float dt);
		void MoveRight(float dt);
		void MoveUpward(float dt);
		void UpdateGUI();

		void ToggleFlyingCam();

		bool m_bUseWireFrameMode;

	private:
		// 카메라 트랜스폼.
		Vector3 m_position = Vector3(0.0f, 0.0f, -2.0f);
		Vector3 m_viewDir = Vector3(0.0f, 0.0f, 1.0f);  // z축 양의 방향 바라봄.
		Vector3 m_upDir = Vector3(0.0f, 1.0f, 0.0f);    // up vector 고정.
		Vector3 m_rightDir = Vector3(1.0f, 0.0f, 0.0f); // 왼손 좌표계에서는 x축 양의 방향이 오른쪽.
		float m_pitch = 0.f;
		float m_yaw = 0.f;

		// Projection 변환 행렬 관련 변수.
		float m_projFovAngleY = 90.0f;
		float m_nearZ = 0.01f;
		float m_farZ = 100.0f;
		float m_aspectRatio = 16.f / 9.f;
		bool m_usePerspectiveProjection = true;
		bool m_useFlyingCam = false;

		float m_speed = 1.0f;
	};

}


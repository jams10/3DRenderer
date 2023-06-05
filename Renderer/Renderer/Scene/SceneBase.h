#pragma once

#include <vector>

#include "Utility/CustomMacros.h"

#include <memory>

#include "Model/ConstantData.h"

/*
*	<SceneBase>
*	��� Scene���� �θ� Ŭ����.
*	Application Ŭ���� �ʿ��� ��� ������ �̿��� ��ĳ������ ���� SceneBase �����͸� ���� ���� ������ �ʱ�ȭ �� ������Ʈ, ������ �Լ��� ȣ���� �� �� �ֵ��� ��.
*/
namespace NS
{
	class GraphicsProcessor;
	class Camera;
	using std::vector;

	class SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics, Camera* pCamera);
		virtual void UpdateGUI();
		virtual void Update(float dt);
		virtual void Render() = 0;
		virtual ~SceneBase();

		void UpdateGlobalConstantData(const Vector3& eyeWorld, const Matrix& viewRow, const Matrix& projRow);

	protected:
		GraphicsProcessor* m_pGraphics;
		Camera* m_pCamera;
		GlobalConstants m_globalConstantBufferData;
	};
}



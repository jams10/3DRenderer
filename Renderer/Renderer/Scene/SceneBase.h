#pragma once

#include <vector>

#include "Utility/CustomMacros.h"

#include <memory>

#include "Model/ConstantData.h"
#include "Model/Model.h"
#include "Object/Light.h"
#include "Object/LightModel.h"

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
		virtual void Render();
		virtual ~SceneBase();

		void UpdateGlobalCameraTransformConstant(const Vector3& eyeWorld, const Matrix& viewRow, const Matrix& projRow);
		void UpdateGlobalSceneDataConstant();

	protected:
		GraphicsProcessor* m_pGraphics;
		Camera* m_pCamera;
		Light m_lights[MAX_LIGHTS];
		LightModel m_lightModels[MAX_LIGHTS];
		GlobalCameraTransformConstant m_globalCameraTransfomConstant;
		GlobalSceneDataConstant m_globalSceneDataConstant;

		bool m_bUseLighting = false;

		MeshForCPU MakeCube(float scale);
	};
}



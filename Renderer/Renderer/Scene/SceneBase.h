#pragma once

#include <vector>

#include "Utility/CustomMacros.h"

#include <memory>

#include "Model/ConstantData.h"

/*
*	<SceneBase>
*	모든 Scene들의 부모 클래스.
*	Application 클래스 쪽에서 상속 구조를 이용한 업캐스팅을 통해 SceneBase 포인터를 통해 여러 씬들의 초기화 및 업데이트, 렌더링 함수를 호출해 줄 수 있도록 함.
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



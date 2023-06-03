#pragma once

#include <vector>

#include "Utility/CustomMacros.h"

#include <memory>

/*
*	<SceneBase>
*	��� Scene���� �θ� Ŭ����.
*	Application Ŭ���� �ʿ��� ��� ������ �̿��� ��ĳ������ ���� SceneBase �����͸� ���� ���� ������ �ʱ�ȭ �� ������Ʈ, ������ �Լ��� ȣ���� �� �� �ֵ��� ��.
*/
namespace NS
{
	class GraphicsProcessor;
	using std::vector;

	class SceneBase
	{
	public:
		virtual void Initialize(GraphicsProcessor* pGraphics);
		virtual void UpdateGUI() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
		virtual ~SceneBase();

	protected:
		GraphicsProcessor* m_pGraphics;
	};
}



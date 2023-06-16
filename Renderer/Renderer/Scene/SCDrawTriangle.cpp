
#include "SCDrawTriangle.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"
#include "Object/Camera.h"

namespace NS
{
	void SCDrawTriangle::Initialize(GraphicsProcessor* pGraphics, Camera* pCamera)
	{
		SceneBase::Initialize(pGraphics, pCamera);

		m_triangleModel.InitializeWithDrawingNormal(pGraphics, std::vector<MeshForCPU>{MeshGenerator::MakeTriangle()});
		m_triangleModel.m_bUseMaterial = false;
		m_triangleModel.m_bUseLightingFactor = false;
	}

	SCDrawTriangle::~SCDrawTriangle()
	{
		m_triangleModel.Shutdown();
	}

	void SCDrawTriangle::UpdateGUI()
	{
		SceneBase::UpdateGUI();

		m_triangleModel.UpdateGUI();
	}

	void SCDrawTriangle::Update(float dt)
	{
		SceneBase::Update(dt);

		// �� ������Ʈ.
		m_triangleModel.Update(dt, m_pGraphics);
	}

	void SCDrawTriangle::Render()
	{
		SceneBase::Render();

		if (m_pCamera->m_bUseWireFrameMode)
		{
			m_pGraphics->SetPipelineState(Graphics::vertexColorWirePSO);
		}
		else
		{
			m_pGraphics->SetPipelineState(Graphics::vertexColorPSO);
		}

		m_triangleModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.

		if (m_triangleModel.m_bDrawNormals)
		{
			m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
			m_triangleModel.RenderNormal(m_pGraphics);
		}
	}
}
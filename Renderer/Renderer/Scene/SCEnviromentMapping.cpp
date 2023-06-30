
#include "SCEnviromentMapping.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"
#include "Object/Camera.h"

namespace NS
{
	void SCEnviromentMapping::Initialize(GraphicsProcessor* pGraphics, Camera* pCamera)
	{
		SceneBase::Initialize(pGraphics, pCamera);

		m_skyBox.Initialize(pGraphics, "..\\Resources\\Textures\\Cubemaps\\NissiBeach2.dds");

		std::vector<MeshForCPU> mesh = MeshGenerator::ReadFromFile("..\\Resources\\Models\\", "skull.obj");
		m_model.InitializeWithDrawingNormal(pGraphics, mesh, m_skyBox.GetTextureResource());

	}

	SCEnviromentMapping::~SCEnviromentMapping()
	{
	}

	void SCEnviromentMapping::UpdateGUI()
	{
		SceneBase::UpdateGUI();

		m_model.UpdateGUI();
	}

	void SCEnviromentMapping::Update(float dt)
	{
		SceneBase::Update(dt);

		// �� ������Ʈ.
		m_model.Update(dt, m_pGraphics);
	}

	void SCEnviromentMapping::Render()
	{
		SceneBase::Render();

		if (m_pCamera->m_bUseWireFrameMode)
		{
			m_pGraphics->SetPipelineState(Graphics::defaultWirePSO);
			m_model.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.
			m_skyBox.Render(m_pGraphics); // ��ī�̹ڽ� �׸���.
		}
		else
		{
			m_pGraphics->SetPipelineState(Graphics::enviromentMappingPSO);
			m_model.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.
			m_pGraphics->SetPipelineState(Graphics::drawingCubeMapPSO);
			m_skyBox.Render(m_pGraphics); // ��ī�̹ڽ� �׸���.
		}

		if (m_model.m_bDrawNormals)
		{
			m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
			m_model.RenderNormal(m_pGraphics);
		}
	}

}

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

		m_triangleModel.Initialize(pGraphics, std::vector<MeshForCPU>{MakeTriangle()});
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

		// 모델 업데이트.
		m_triangleModel.Update(dt, m_pGraphics);
	}

	void SCDrawTriangle::Render()
	{
		SceneBase::Render();

		// 샘플러 바인딩.
		m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());
		m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());

		if (m_pCamera->m_bUseWireFrameMode)
		{
			m_pGraphics->SetPipelineState(Graphics::vertexColorWirePSO);
		}
		else
		{
			m_pGraphics->SetPipelineState(Graphics::vertexColorPSO);
		}

		m_triangleModel.Render(m_pGraphics); // 모델 정점, 인덱스 버퍼, 상수 버퍼 바인딩.
	}

	MeshForCPU SCDrawTriangle::MakeTriangle()
	{
		MeshForCPU meshForCPU;
		vector<Vector3> positions;
		vector<Vector3> normalModel;
		vector<Vector2> texcoord;
		vector<Vector3> tangentModel;
		vector<Vector3> colors;

		positions.push_back(Vector3(-1.f, -1.f, 0.f));
		positions.push_back(Vector3(0.f, 1.f, 0.f));
		positions.push_back(Vector3(1.f, -1.f, 0.f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		texcoord.push_back(Vector2(0.0f, 1.0f));
		texcoord.push_back(Vector2(0.5f, 0.0f));
		texcoord.push_back(Vector2(0.1f, 1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

		for (int i = 0; i < 3; ++i)
		{
			Vertex v;
			v.position = positions[i];
			v.normalModel = normalModel[i];
			v.texcoord = texcoord[i];
			v.tangentModel = tangentModel[i];
			v.color = colors[i];
			meshForCPU.vertices.push_back(v);
		}

		meshForCPU.indices = { 0, 1, 2, };
		return meshForCPU;
	}
}

#include "SCDrawTriangle.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SCDrawTriangle::Initialize(GraphicsProcessor* pGraphics)
	{
		SceneBase::Initialize(pGraphics);

		m_triangleModel.Initialize(pGraphics, std::vector<MeshForCPU>{MakeTriangle()});

		// 전역 상수 버퍼 생성.
		pGraphics->GetD3D11()->CreateGlobalConstantBuffer(m_globalConstantBufferData);
	}

	SCDrawTriangle::~SCDrawTriangle()
	{
		m_triangleModel.m_meshes[0]->Shutdown();
	}

	void SCDrawTriangle::UpdateGUI()
	{
		SceneBase::UpdateGUI();

		ImGui::Begin("Position");

		ImGui::SliderFloat3("Translation", &m_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scaling", &m_scale.x, 0.1f, 2.0f);

		ImGui::End();
	}

	void SCDrawTriangle::Update(float dt)
	{
		SceneBase::Update(dt);

		/* 글로벌 상수 버퍼랑 모델 별 상수 버퍼 나눠서 쉐이더에 정의하고 cpu쪽에서 생성, 바인딩 해주어야 함.*/

		// 월드 변환 행렬. SRT 순서.
		m_triangleModel.m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_translation);
		// 모델 트랜스폼 상수 버퍼 업데이트.
		m_triangleModel.UpdateModelTransformConstantBuffer(m_pGraphics);

		// 글로벌 상수 버퍼 데이터 업데이트.(CPU)
		UpdateGlobalConstantData(
			m_camera.GetCameraPosition(), 
			m_camera.GetViewMatrixRow(), 
			m_camera.GetProjectionMatrixRow()
		);

		// 글로벌 상수 버퍼 업데이트.(GPU)
		m_pGraphics->GetD3D11()->UpdateGlobalConstantBuffer(m_globalConstantBufferData);
	}

	void SCDrawTriangle::Render()
	{
		// 샘플러 바인딩.
		m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());
		m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());

		m_pGraphics->SetPipelineState(Graphics::vertexColorPSO);
		// 전역 상수 버퍼 파이프라인에 세팅.
		m_pGraphics->GetD3D11()->SetGlobalConstantBufferData();

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
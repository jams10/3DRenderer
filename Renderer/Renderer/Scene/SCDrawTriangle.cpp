
#include "SCDrawTriangle.h"

#include <imgui.h>

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SCDrawTriangle::Initialize(Graphics* pGraphics)
	{
		SceneBase::Initialize(pGraphics);

		MakeTriangle();

		// 정점 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateVertexBuffer(m_meshForCPU.vertices, m_meshForGPU.pVertexBuffer);

		// 인덱스 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateIndexBuffer(m_meshForCPU.indices, m_meshForGPU.pIndexBuffer);

		// 상수 버퍼 생성.
		m_constantData.world = Matrix{};
		m_constantData.view = Matrix{};
		m_constantData.projection = Matrix{};
		m_pGraphics->GetD3D11()->CreateConstantBuffer(m_constantData, m_meshForGPU.pVertexConstantBuffer);

		// 정점 레이아웃, 쉐이더 생성. 레이아웃은 반드시 정점 구조와 맞춰 주어야 함.
		vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\ColorVertexShader.hlsl", inputElements,
			m_meshForGPU.pVertexShader, m_meshForGPU.pInputLayout);

		// 픽셀 쉐이더 생성.
		m_pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\ColorPixelShader.hlsl", m_meshForGPU.pPixelShader);
	}

	void SCDrawTriangle::UpdateGUI()
	{
		ImGui::Begin("Position");

		ImGui::SliderFloat3("m_modelTranslation", &m_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("m_modelRotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("m_modelScaling", &m_scale.x, 0.1f, 2.0f);

		ImGui::End();
	}

	void SCDrawTriangle::Update(float dt)
	{
		// 월드 변환 행렬. SRT 순서.
		m_constantData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_translation);
		m_constantData.world = m_constantData.world.Transpose(); // GPU로 넘겨주기 전에 colum major로 바꿔주기.

		// 뷰 변환 행렬.
		m_constantData.view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		m_constantData.view = m_constantData.view.Transpose();

		// 투영 변환 행렬.
		const float aspect = m_pGraphics->GetD3D11()->GetAspectRatio();
		const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		m_constantData.projection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		m_constantData.projection = m_constantData.projection.Transpose();

		m_pGraphics->GetD3D11()->m_drawAsWireFrame = false;

		// 업데이트 한 변환 행렬을 CPU에서 GPU로 보내 업데이트.
		m_pGraphics->GetD3D11()->UpdateBuffer(m_constantData, m_meshForGPU.pVertexConstantBuffer);
	}

	void SCDrawTriangle::Render()
	{
		m_pGraphics->GetD3D11()->Render(m_meshForCPU, m_meshForGPU);
	}

	void SCDrawTriangle::MakeTriangle()
	{
		vector<Vector3> positions;
		vector<Vector3> colors;

		positions.push_back(Vector3(-1.f, -1.f, 0.f));
		positions.push_back(Vector3(0.f, 1.f, 0.f));
		positions.push_back(Vector3(1.f, -1.f, 0.f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

		for (int i = 0; i < 3; ++i)
		{
			m_meshForCPU.vertices.push_back(ColorVertex{ positions[i], colors[i] });
		}

		m_meshForCPU.indices = { 0, 1, 2, };
		m_meshForCPU.indexCount = 3;
	}
}
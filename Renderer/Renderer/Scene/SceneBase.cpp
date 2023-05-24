#include "SceneBase.h"

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"

namespace NS
{
	void SceneBase::Initialize(Graphics* pGraphics)
	{
		m_pGraphics = pGraphics;

		MakeTriangle();

		// 정점 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateVertexBuffer(m_triangleMeshForCPU.m_vertices, m_triangleMeshForGPU.m_pVertexBuffer);

		// 인덱스 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateIndexBuffer(m_triangleMeshForCPU.m_indices, m_triangleMeshForGPU.m_pIndexBuffer);

		// 상수 버퍼 생성.
		m_triangleConstantData.model = Matrix{};
		m_triangleConstantData.view = Matrix{};
		m_triangleConstantData.projection = Matrix{};
		m_pGraphics->GetD3D11()->CreateConstantBuffer(m_triangleConstantData, m_triangleMeshForGPU.m_pConstantBuffer);

		// 정점 레이아웃, 쉐이더 생성. 레이아웃은 반드시 정점 구조와 맞춰 주어야 함.
		vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\ColorVertexShader.hlsl", inputElements,
			m_triangleMeshForGPU.m_pVertexShader, m_triangleMeshForGPU.m_pInputLayout);

		// 픽셀 쉐이더 생성.
		m_pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\ColorPixelShader.hlsl", m_triangleMeshForGPU.m_pPixelShader);
	}

	void SceneBase::Update(float dt)
	{
		static float rotation = 0.0f;
		rotation += dt / 1000.0f;
		if (rotation >= 360.f) rotation = 0.f;

		// 월드 변환 행렬. SRT 순서.
		m_triangleConstantData.model = Matrix::CreateScale(0.5f) * Matrix::CreateRotationZ(rotation) *
			Matrix::CreateTranslation(Vector3(0.f, 0.0f, 1.0f));
		m_triangleConstantData.model = m_triangleConstantData.model.Transpose(); // GPU로 넘겨주기 전에 colum major로 바꿔주기.

		// 뷰 변환 행렬.
		m_triangleConstantData.view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		m_triangleConstantData.view = m_triangleConstantData.view.Transpose();

		// 투영 변환 행렬.
		const float aspect = m_pGraphics->GetD3D11()->GetAspectRatio();
		const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		m_triangleConstantData.projection =
			DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		m_triangleConstantData.projection = m_triangleConstantData.projection.Transpose();

		// 업데이트 한 변환 행렬을 CPU에서 GPU로 보내 업데이트.
		m_pGraphics->GetD3D11()->UpdateBuffer(m_triangleConstantData, m_triangleMeshForGPU.m_pConstantBuffer);
	}

	void SceneBase::Render()
	{
		m_pGraphics->GetD3D11()->Render(m_triangleMeshForCPU, m_triangleMeshForGPU);
	}

	void SceneBase::MakeTriangle()
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
			m_triangleMeshForCPU.m_vertices.push_back(Vertex{ positions[i], colors[i] });
		}

		m_triangleMeshForCPU.m_indices = { 0, 1, 2, };
		m_triangleMeshForCPU.m_indexCount = 3;
	}
}
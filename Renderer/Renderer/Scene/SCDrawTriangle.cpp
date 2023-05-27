
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

		// ���� ���� ����.
		m_pGraphics->GetD3D11()->CreateVertexBuffer(m_cubeMeshForCPU.vertices, m_cubeMeshForGPU.pVertexBuffer);

		// �ε��� ���� ����.
		m_pGraphics->GetD3D11()->CreateIndexBuffer(m_cubeMeshForCPU.indices, m_cubeMeshForGPU.pIndexBuffer);

		// ��� ���� ����.
		m_cubeConstantData.model = Matrix{};
		m_cubeConstantData.view = Matrix{};
		m_cubeConstantData.projection = Matrix{};
		m_pGraphics->GetD3D11()->CreateConstantBuffer(m_cubeConstantData, m_cubeMeshForGPU.pConstantBuffer);

		// ���� ���̾ƿ�, ���̴� ����. ���̾ƿ��� �ݵ�� ���� ������ ���� �־�� ��.
		vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\ColorVertexShader.hlsl", inputElements,
			m_cubeMeshForGPU.pVertexShader, m_cubeMeshForGPU.pInputLayout);

		// �ȼ� ���̴� ����.
		m_pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\ColorPixelShader.hlsl", m_cubeMeshForGPU.pPixelShader);
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
		// ���� ��ȯ ���. SRT ����.
		m_cubeConstantData.model = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_translation);
		m_cubeConstantData.model = m_cubeConstantData.model.Transpose(); // GPU�� �Ѱ��ֱ� ���� colum major�� �ٲ��ֱ�.

		// �� ��ȯ ���.
		m_cubeConstantData.view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		m_cubeConstantData.view = m_cubeConstantData.view.Transpose();

		// ���� ��ȯ ���.
		const float aspect = m_pGraphics->GetD3D11()->GetAspectRatio();
		const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		m_cubeConstantData.projection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		m_cubeConstantData.projection = m_cubeConstantData.projection.Transpose();

		// ������Ʈ �� ��ȯ ����� CPU���� GPU�� ���� ������Ʈ.
		m_pGraphics->GetD3D11()->UpdateBuffer(m_cubeConstantData, m_cubeMeshForGPU.pConstantBuffer);
	}

	void SCDrawTriangle::Render()
	{
		m_pGraphics->GetD3D11()->Render(m_cubeMeshForCPU, m_cubeMeshForGPU);
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
			m_cubeMeshForCPU.vertices.push_back(Vertex{ positions[i], colors[i] });
		}

		m_cubeMeshForCPU.indices = { 0, 1, 2, };
		m_cubeMeshForCPU.indexCount = 3;
	}
}
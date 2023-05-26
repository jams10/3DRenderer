#include "SceneBase.h"

#include <imgui.h>

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SceneBase::Initialize(Graphics* pGraphics)
	{
		m_pGraphics = pGraphics;

		MakeCube();

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

	void SceneBase::UpdateGUI()
	{
		ImGui::Begin("Position");

		ImGui::SliderFloat3("m_modelTranslation", &m_translation.x, -2.0f, 2.0f);
		ImGui::SliderFloat3("m_modelRotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("m_modelScaling", &m_scale.x, 0.1f, 2.0f);

		ImGui::Checkbox("usePerspectiveProjection", &bUsePerspective);
		ImGui::End();
	}

	void SceneBase::Update(float dt)
	{
		static float rotation = 0.0f;
		rotation += dt / 1000.0f;
		if (rotation >= 360.f) rotation = 0.f;

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
		if (bUsePerspective) 
		{
			const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
			m_cubeConstantData.projection =
				DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		}
		else 
		{
			m_cubeConstantData.projection =
				DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
		}
		m_cubeConstantData.projection = m_cubeConstantData.projection.Transpose();

		// ������Ʈ �� ��ȯ ����� CPU���� GPU�� ���� ������Ʈ.
		m_pGraphics->GetD3D11()->UpdateBuffer(m_cubeConstantData, m_cubeMeshForGPU.pConstantBuffer);
	}

	void SceneBase::Render()
	{
		m_pGraphics->GetD3D11()->Render(m_cubeMeshForCPU, m_cubeMeshForGPU);
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
			m_cubeMeshForCPU.vertices.push_back(Vertex{ positions[i], colors[i] });
		}

		m_cubeMeshForCPU.indices = { 0, 1, 2, };
		m_cubeMeshForCPU.indexCount = 3;
	}

	void SceneBase::MakeCube()
	{
		vector<Vector3> positions;
		vector<Vector3> colors;

		// ���� 
		positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
		positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
		positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
		positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));

        // �Ʒ���
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));

        // �ո�
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

        // �޸�
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));

        // ����
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));

        // ������
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));

        for (size_t i = 0; i < positions.size(); i++) 
		{
			m_cubeMeshForCPU.vertices.push_back(Vertex{ positions[i], colors[i] });
        }

		m_cubeMeshForCPU.indices = {
            0,  1,  2,  0,  2,  3,  // ����
            4,  5,  6,  4,  6,  7,  // �Ʒ���
            8,  9,  10, 8,  10, 11, // �ո�
            12, 13, 14, 12, 14, 15, // �޸�
            16, 17, 18, 16, 18, 19, // ����
            20, 21, 22, 20, 22, 23  // ������
        };

		m_cubeMeshForCPU.indexCount = 36;
	}
}
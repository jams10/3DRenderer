
#include "SCDrawTexturedCube.h"

#include <imgui.h>

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SCDrawTexturedCube::Initialize(Graphics* pGraphics)
	{
		SceneBase::Initialize(pGraphics);

		MakeCube(1.0f);
		m_meshForCPU.textureFilePath = "..\\Resources\\Textures\\PaperBox.jpg";

		// 정점 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateVertexBuffer(m_meshForCPU.vertices, m_meshForGPU.pVertexBuffer);

		// 인덱스 버퍼 생성.
		m_pGraphics->GetD3D11()->CreateIndexBuffer(m_meshForCPU.indices, m_meshForGPU.pIndexBuffer);

		// 상수 버퍼 생성.
		m_constantData.model = Matrix{};
		m_constantData.invTranspose = Matrix{};
		m_constantData.view = Matrix{};
		m_constantData.projection = Matrix{};
		m_pGraphics->GetD3D11()->CreateConstantBuffer(m_constantData, m_meshForGPU.pConstantBuffer);

		// 정점 레이아웃, 쉐이더 생성. 레이아웃은 반드시 정점 구조와 맞춰 주어야 함.
		vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\TextureVertexShader.hlsl", inputElements,
			m_meshForGPU.pVertexShader, m_meshForGPU.pInputLayout);

		// 픽셀 쉐이더 생성.
		m_pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\TexturePixelShader.hlsl", m_meshForGPU.pPixelShader);

		// 텍스쳐 파일 생성.
		m_pGraphics->GetD3D11()->CreateTexture(m_meshForCPU.textureFilePath, m_meshForGPU.pDiffuseTexture, m_meshForGPU.pDiffuseMapSRV);
	}

	void SCDrawTexturedCube::UpdateGUI()
	{
		ImGui::Begin("Position");

		ImGui::SliderFloat3("m_modelTranslation", &m_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("m_modelRotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("m_modelScaling", &m_scale.x, 0.1f, 2.0f);

		ImGui::End();
	}

	void SCDrawTexturedCube::Update(float dt)
	{
		// 월드 변환 행렬. SRT 순서.
		m_constantData.model = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_translation);
		m_constantData.model = m_constantData.model.Transpose(); // GPU로 넘겨주기 전에 colum major로 바꿔주기.

        m_constantData.invTranspose = m_constantData.model;
        m_constantData.invTranspose.Translation(Vector3(0.0f));
        m_constantData.invTranspose = m_constantData.invTranspose.Invert().Transpose();

		// 뷰 변환 행렬.
		m_constantData.view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		m_constantData.view = m_constantData.view.Transpose();

		// 투영 변환 행렬.
		const float aspect = m_pGraphics->GetD3D11()->GetAspectRatio();
		const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		m_constantData.projection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		m_constantData.projection = m_constantData.projection.Transpose();

		// 업데이트 한 변환 행렬을 CPU에서 GPU로 보내 업데이트.
		m_pGraphics->GetD3D11()->UpdateBuffer(m_constantData, m_meshForGPU.pConstantBuffer);
	}

	void SCDrawTexturedCube::Render()
	{
		m_pGraphics->GetD3D11()->Render(m_meshForCPU, m_meshForGPU);
	}

	void SCDrawTexturedCube::MakeCube(float scale)
	{
        vector<Vector3> positions;
        vector<Vector3> normals;
        vector<Vector2> texcoords;

        // 윗면
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 아랫면
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 앞면
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 뒷면
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 왼쪽
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 오른쪽
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        for (size_t i = 0; i < positions.size(); i++) 
        {
            m_meshForCPU.vertices.push_back(BasicVertex{ positions[i], normals[i], texcoords[i] });
        }

        m_meshForCPU.indices = {
            0,  1,  2,  0,  2,  3,  // 윗면
            4,  5,  6,  4,  6,  7,  // 아랫면
            8,  9,  10, 8,  10, 11, // 앞면
            12, 13, 14, 12, 14, 15, // 뒷면
            16, 17, 18, 16, 18, 19, // 왼쪽
            20, 21, 22, 20, 22, 23  // 오른쪽
        };

        m_meshForCPU.indexCount = m_meshForCPU.indices.size();
    }
}

#include "SCDrawTexturedCube.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SCDrawTexturedCube::Initialize(GraphicsProcessor* pGraphics)
	{
		SceneBase::Initialize(pGraphics);

        MeshForCPU cube = MakeCube(1.0f);
        cube.albedoTextureFilename = "..\\Resources\\Textures\\PaperBox.jpg";
        m_cubeModel.Initialize(pGraphics, std::vector<MeshForCPU>{cube});

        // ���� ��� ���� ����.
        pGraphics->GetD3D11()->CreateGlobalConstantBuffer(m_globalConstantBufferData);
	}

    SCDrawTexturedCube::~SCDrawTexturedCube()
    {
        m_cubeModel.m_meshes[0]->Shutdown();
    }

	void SCDrawTexturedCube::UpdateGUI()
	{
		ImGui::Begin("Position");

		ImGui::SliderFloat3("m_modelTranslation", &m_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("m_modelRotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("m_modelScaling", &m_scale.x, 0.1f, 2.0f);
        ImGui::Checkbox("Use Perspective", &m_bUsePerspectiveProjection);
        ImGui::Checkbox("Use Wireframe", &m_bUseWireFrame);

		ImGui::End();
	}

	void SCDrawTexturedCube::Update(float dt)
	{
        // ���� ��ȯ ���. SRT ����.
        m_cubeModel.m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
            Matrix::CreateRotationY(m_rotation.y) *
            Matrix::CreateRotationX(m_rotation.x) *
            Matrix::CreateRotationZ(m_rotation.z) *
            Matrix::CreateTranslation(m_translation);
        // �� Ʈ������ ��� ���� ������Ʈ.
        m_cubeModel.UpdateModelTransformConstantBuffer(m_pGraphics);

		// �� ��ȯ ���.
        m_globalConstantBufferData.view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
        m_globalConstantBufferData.view = m_globalConstantBufferData.view.Transpose();

		// ���� ��ȯ ���.
		const float aspect = m_pGraphics->GetD3D11()->GetAspectRatio();
        if (m_bUsePerspectiveProjection) 
        {
            const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
            m_globalConstantBufferData.proj =
                DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
        }
        else 
        {
            m_globalConstantBufferData.proj =
                DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
        }
        m_globalConstantBufferData.proj = m_globalConstantBufferData.proj.Transpose();

        // �۷ι� ��� ���� ������Ʈ.
        m_pGraphics->GetD3D11()->UpdateGlobalConstantBuffer(m_globalConstantBufferData);
	}

	void SCDrawTexturedCube::Render()
	{
        // ���÷� ���ε�.
        m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());
        m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());

        if (m_bUseWireFrame)
        {
            m_pGraphics->SetPipelineState(Graphics::defaultWirePSO);
        }
        else
        {
            m_pGraphics->SetPipelineState(Graphics::textureMappingPSO);
        }

        // ���� ��� ���� ���������ο� ����.
        m_pGraphics->GetD3D11()->SetGlobalConstantBufferData();

        m_cubeModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.
	}

    MeshForCPU SCDrawTexturedCube::MakeCube(float scale)
	{
        MeshForCPU meshForCPU;
        vector<Vector3> positions;
        vector<Vector3> normals;
        vector<Vector2> texcoords;

        // ����
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

        // �Ʒ���
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

        // �ո�
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

        // �޸�
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

        // ����
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

        // ������
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
            Vertex v;
            v.position = positions[i];
            v.normalModel = normals[i];
            v.texcoord = texcoords[i];
            v.tangentModel = Vector3(0, 0, 0);
            v.color = Vector3(0, 0, 0);
            meshForCPU.vertices.push_back(v);
        }

        meshForCPU.indices = {
            0,  1,  2,  0,  2,  3,  // ����
            4,  5,  6,  4,  6,  7,  // �Ʒ���
            8,  9,  10, 8,  10, 11, // �ո�
            12, 13, 14, 12, 14, 15, // �޸�
            16, 17, 18, 16, 18, 19, // ����
            20, 21, 22, 20, 22, 23  // ������
        };

        return meshForCPU;
    }
}
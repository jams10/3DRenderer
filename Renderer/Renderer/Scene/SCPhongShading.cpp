
#include "SCPhongShading.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"
#include "Object/Camera.h"

namespace NS
{
    void SCPhongShading::Initialize(GraphicsProcessor* pGraphics, Camera* pCamera)
    {
        SceneBase::Initialize(pGraphics, pCamera);

        MeshForCPU cube = MakeCube(1.0f);
        cube.albedoTextureFilename = "..\\Resources\\Textures\\PaperBox.jpg";
        m_cubeModel.Initialize(pGraphics, std::vector<MeshForCPU>{cube});

        // ���� ��� ���� ����.
        pGraphics->GetD3D11()->CreateGlobalConstantBuffer(m_globalConstantBufferData);
    }

    SCPhongShading::~SCPhongShading()
    {
        m_cubeModel.Shutdown();
    }

    void SCPhongShading::UpdateGUI()
    {
        SceneBase::UpdateGUI();

        m_cubeModel.UpdateGUI();
    }

    void SCPhongShading::Update(float dt)
    {
        SceneBase::Update(dt);

        // �� ������Ʈ.
        m_cubeModel.Update(dt, m_pGraphics);

        // �۷ι� ��� ���� ������ ������Ʈ.(CPU)
        UpdateGlobalConstantData(
            m_pCamera->GetCameraPosition(),
            m_pCamera->GetViewMatrixRow(),
            m_pCamera->GetProjectionMatrixRow()
        );

        // �۷ι� ��� ���� ������Ʈ.(GPU)
        m_pGraphics->GetD3D11()->UpdateGlobalConstantBuffer(m_globalConstantBufferData);
    }

    void SCPhongShading::Render()
    {
        // ���÷� ���ε�.
        m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());
        m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());

        if (m_pCamera->m_bUseWireFrameMode)
        {
            m_pGraphics->SetPipelineState(Graphics::defaultWirePSO);
        }
        else
        {
            m_pGraphics->SetPipelineState(Graphics::phongShadingPSO);
        }

        // ���� ��� ���� ���������ο� ����.
        m_pGraphics->GetD3D11()->SetGlobalConstantBufferData();

        m_cubeModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.
    }

    MeshForCPU SCPhongShading::MakeCube(float scale)
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
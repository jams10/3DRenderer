
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

        MeshForCPU sphere = MeshGenerator::SubdivideToSphere(1.f, MeshGenerator::MakeBox(1.f));
        sphere = MeshGenerator::SubdivideToSphere(1.f, sphere);
        sphere.albedoTextureFilename = "..\\Resources\\Textures\\PaperBox.jpg";
        m_sphereModel.InitializeWithDrawingNormal(pGraphics, std::vector<MeshForCPU>{sphere});

        m_bUseLighting = true;
    }

    SCPhongShading::~SCPhongShading()
    {
        m_sphereModel.Shutdown();
    }

    void SCPhongShading::UpdateGUI()
    {
        SceneBase::UpdateGUI();

        m_sphereModel.UpdateGUI();
    }

    void SCPhongShading::Update(float dt)
    {
        SceneBase::Update(dt);

        // �� ������Ʈ.
        m_sphereModel.Update(dt, m_pGraphics);
    }

    void SCPhongShading::Render()
    {
        SceneBase::Render();

        if (m_pCamera->m_bUseWireFrameMode)
        {
            m_pGraphics->SetPipelineState(Graphics::defaultWirePSO);
        }
        else
        {
            m_pGraphics->SetPipelineState(Graphics::phongShadingPSO);
        }

        m_sphereModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.

        if (m_sphereModel.m_bDrawNormals)
        {
            m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
            m_sphereModel.RenderNormal(m_pGraphics);
        }
    }
}

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

        std::vector<MeshForCPU> mesh = MeshGenerator::ReadFromFile("..\\Resources\\Models\\", "skull.obj");
        mesh[0].textures.albedoTextureFilename = "..\\Resources\\Textures\\Skull\\difuso_flip_oscuro.jpg";
        m_model.InitializeWithDrawingNormal(pGraphics, mesh);

        m_bUseLighting = true;
    }

    SCPhongShading::~SCPhongShading()
    {
        m_model.Shutdown();
    }

    void SCPhongShading::UpdateGUI()
    {
        SceneBase::UpdateGUI();

        m_model.UpdateGUI();
    }

    void SCPhongShading::Update(float dt)
    {
        SceneBase::Update(dt);

        // 모델 업데이트.
        m_model.Update(dt, m_pGraphics);
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

        m_model.Render(m_pGraphics); // 모델 정점, 인덱스 버퍼, 상수 버퍼 바인딩.

        if (m_model.m_bDrawNormals)
        {
            m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
            m_model.RenderNormal(m_pGraphics);
        }
    }
}
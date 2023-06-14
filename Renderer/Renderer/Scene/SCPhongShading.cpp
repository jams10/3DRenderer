
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
        m_cubeModel.InitializeWithDrawingNormal(pGraphics, std::vector<MeshForCPU>{cube});

        m_bUseLighting = true;
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

        m_cubeModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.

        if (m_cubeModel.m_bDrawNormals)
        {
            m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
            m_cubeModel.RenderNormal(m_pGraphics);
        }
    }
}

#include "SCDrawTexturedCube.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"
#include "Object/Camera.h"

namespace NS
{
    void SCDrawTexturedCube::Initialize(GraphicsProcessor* pGraphics, Camera* pCamera)
    {
        SceneBase::Initialize(pGraphics, pCamera);

        MeshForCPU cube = MeshGenerator::MakeBox();
        cube.textures.albedoTextureFilename = "..\\Resources\\Textures\\PaperBox.jpg";
        m_cubeModel.InitializeWithDrawingNormal(pGraphics, std::vector<MeshForCPU>{cube});
        m_cubeModel.m_bUseLightingFactor = false;
	}

    SCDrawTexturedCube::~SCDrawTexturedCube()
    {
    }

	void SCDrawTexturedCube::UpdateGUI()
	{
        SceneBase::UpdateGUI();

        m_cubeModel.UpdateGUI();

	}

	void SCDrawTexturedCube::Update(float dt)
	{
        SceneBase::Update(dt);

        // 모델 업데이트.
        m_cubeModel.Update(dt, m_pGraphics);
	}

	void SCDrawTexturedCube::Render()
	{
        SceneBase::Render();

        if (m_pCamera->m_bUseWireFrameMode)
        {
            m_pGraphics->SetPipelineState(Graphics::defaultWirePSO);
        }
        else
        {
            m_pGraphics->SetPipelineState(Graphics::textureMappingPSO);
        }

        m_cubeModel.Render(m_pGraphics); // 모델 정점, 인덱스 버퍼, 상수 버퍼 바인딩.

        if (m_cubeModel.m_bDrawNormals)
        {
            m_pGraphics->SetPipelineState(Graphics::drawingNormalPSO);
            m_cubeModel.RenderNormal(m_pGraphics);
        }
	}
}
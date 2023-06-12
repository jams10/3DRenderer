
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

        MeshForCPU cube = MakeCube(1.0f);
        cube.albedoTextureFilename = "..\\Resources\\Textures\\PaperBox.jpg";
        m_cubeModel.Initialize(pGraphics, std::vector<MeshForCPU>{cube});
        m_cubeModel.m_bUseLightingFactor = false;
	}

    SCDrawTexturedCube::~SCDrawTexturedCube()
    {
        m_cubeModel.Shutdown();
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
	}
}
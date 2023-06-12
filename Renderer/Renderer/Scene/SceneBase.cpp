#include "SceneBase.h"

#include <imgui.h>
#include <iostream>

#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsProcessor.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"
#include "Object/Camera.h"

namespace NS
{
	void SceneBase::Initialize(GraphicsProcessor* pGraphics, Camera* pCamera)
	{
		if (pGraphics == nullptr)
		{
			std::cout << "Initialize Scene : GraphicsProcessor is nullptr.\n";
			return;
		}
		m_pGraphics = pGraphics;
		m_pCamera = pCamera;

		for (int i = 0; i < MAX_LIGHTS; ++i)
		{
			m_lights[i].SetID(i);
            m_lightModels[i].Initialize(pGraphics, std::vector<MeshForCPU>{MakeCube(0.25f)});
		}

		// 전역 상수 버퍼 생성.
		pGraphics->GetD3D11()->CreateGlobalCameraTransformConstantBuffer(m_globalCameraTransfomConstant);
		pGraphics->GetD3D11()->CreateGlobalSceneDataConstantBuffer(m_globalSceneDataConstant);
	};

	SceneBase::~SceneBase()
	{
        for (int i = 0; i < MAX_LIGHTS; ++i)
            m_lightModels[i].Shutdown();
	}

	void SceneBase::UpdateGlobalCameraTransformConstant(const Vector3& eyeWorld, const Matrix& viewRow, const Matrix& projRow)
	{
		m_globalCameraTransfomConstant.eyeWorld = eyeWorld;
		m_globalCameraTransfomConstant.view = viewRow.Transpose();
		m_globalCameraTransfomConstant.proj = projRow.Transpose();
		m_globalCameraTransfomConstant.invProj = projRow.Invert().Transpose();
		m_globalCameraTransfomConstant.viewProj = (viewRow * projRow).Transpose();
	}

	void SceneBase::UpdateGlobalSceneDataConstant()
	{
		if (m_bUseLighting)
		{
			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				m_globalSceneDataConstant.lights[i] = m_lights[i].m_lightConstant;
			}
		}
	}

	void SceneBase::UpdateGUI()
	{
		m_pCamera->UpdateGUI();

		if (m_bUseLighting)
		{
			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				m_lights[i].UpdateGUI();
			}
		}
	}

	void SceneBase::Update(float dt)
	{
		if (m_bUseLighting)
		{
			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				m_lights[i].Update(dt, &m_lightModels[i]);
                if (m_lights[i].bTurnOn)
                    m_lightModels[i].Update(dt, m_pGraphics);
			}
		}

		// 글로벌 상수 버퍼 데이터 업데이트.(CPU)
		UpdateGlobalCameraTransformConstant(
			m_pCamera->GetCameraPosition(),
			m_pCamera->GetViewMatrixRow(),
			m_pCamera->GetProjectionMatrixRow()
		);

		UpdateGlobalSceneDataConstant();

		// 글로벌 상수 버퍼 업데이트.(GPU)
		m_pGraphics->GetD3D11()->UpdateGlobalCameraTransformConstantBuffer(m_globalCameraTransfomConstant);
		m_pGraphics->GetD3D11()->UpdateGlobalSceneDataConstantBuffer(m_globalSceneDataConstant);
	}

	void SceneBase::Render()
	{
		// 전역 상수 버퍼 파이프라인에 세팅.
		m_pGraphics->GetD3D11()->SetGlobalConstantBuffers();

        // 샘플러 바인딩.
        m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());
        m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
            Graphics::samplerStates.data());

        m_pGraphics->SetPipelineState(Graphics::textureMappingPSO);

        if (m_bUseLighting)
        {
            for (int i = 0; i < MAX_LIGHTS; ++i)
            {
                if (m_lights[i].bTurnOn)
                    m_lightModels[i].Render(m_pGraphics);
            }
        }
	}

    MeshForCPU SceneBase::MakeCube(float scale)
    {
        MeshForCPU meshForCPU;
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
            Vertex v;
            v.position = positions[i];
            v.normalModel = normals[i];
            v.texcoord = texcoords[i];
            v.tangentModel = Vector3(0, 0, 0);
            v.color = Vector3(0, 0, 0);
            meshForCPU.vertices.push_back(v);
        }

        meshForCPU.indices = {
            0,  1,  2,  0,  2,  3,  // 윗면
            4,  5,  6,  4,  6,  7,  // 아랫면
            8,  9,  10, 8,  10, 11, // 앞면
            12, 13, 14, 12, 14, 15, // 뒷면
            16, 17, 18, 16, 18, 19, // 왼쪽
            20, 21, 22, 20, 22, 23  // 오른쪽
        };

        return meshForCPU;
    }
}
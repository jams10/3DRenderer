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
            m_lightModels[i].Initialize(pGraphics, std::vector<MeshForCPU>{MeshGenerator::MakeBox(0.25f)});
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

		ImGui::Begin("Scene");

		if (m_bUseLighting)
		{
			ImGui::Checkbox("Use Rim Light", &m_bUseRimLighting);
			m_globalSceneDataConstant.useRimLighthing = m_bUseRimLighting;
			// Rim Lighting 상수 버퍼 CPU 값 업데이트.
			if (m_bUseRimLighting)
			{
				ImGui::ColorEdit3("Rim Light Color", &m_globalSceneDataConstant.rimColor.x);
				ImGui::SliderFloat("rimPower", &m_globalSceneDataConstant.rimPower, 1.f, 5.0f);
				ImGui::SliderFloat("rimStrength", &m_globalSceneDataConstant.rimStrength, 0.f, 5.0f);
			}

			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				m_lights[i].UpdateGUI();
			}
		}

		ImGui::End();
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
}
#include "SceneBase.h"

#include <imgui.h>
#include <iostream>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
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

		// ���� ��� ���� ����.
		pGraphics->GetD3D11()->CreateGlobalCameraTransformConstantBuffer(m_globalCameraTransfomConstant);
		pGraphics->GetD3D11()->CreateGlobalSceneDataConstantBuffer(m_globalSceneDataConstant);
	};

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::UpdateGlobalConstantDataForVS(const Vector3& eyeWorld, const Matrix& viewRow, const Matrix& projRow)
	{
		m_globalCameraTransfomConstant.eyeWorld = eyeWorld;
		m_globalCameraTransfomConstant.view = viewRow.Transpose();
		m_globalCameraTransfomConstant.proj = projRow.Transpose();
		m_globalCameraTransfomConstant.invProj = projRow.Invert().Transpose();
		m_globalCameraTransfomConstant.viewProj = (viewRow * projRow).Transpose();
	}

	void SceneBase::UpdateGUI()
	{
		m_pCamera->UpdateGUI();
	}

	void SceneBase::Update(float dt)
	{
		// �۷ι� ��� ���� ������ ������Ʈ.(CPU)
		UpdateGlobalConstantDataForVS(
			m_pCamera->GetCameraPosition(),
			m_pCamera->GetViewMatrixRow(),
			m_pCamera->GetProjectionMatrixRow()
		);

		// �۷ι� ��� ���� ������Ʈ.(GPU)
		m_pGraphics->GetD3D11()->UpdateGlobalCameraTransformConstantBuffer(m_globalCameraTransfomConstant);
		m_pGraphics->GetD3D11()->UpdateGlobalSceneDataConstantBufferS(m_globalSceneDataConstant);
	}

	void SceneBase::Render()
	{
		// ���� ��� ���� ���������ο� ����.
		m_pGraphics->GetD3D11()->SetGlobalConstantBuffers();
	}
}
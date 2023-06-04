#include "SceneBase.h"

#include <imgui.h>
#include <iostream>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SceneBase::Initialize(GraphicsProcessor* pGraphics)
	{
		if (pGraphics == nullptr)
		{
			std::cout << "Initialize Scene : GraphicsProcessor is nullptr.\n";
			return;
		}
		m_pGraphics = pGraphics;

		m_camera.SetAspectRatio(pGraphics->GetD3D11()->GetAspectRatio());
	};

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::UpdateGlobalConstantData(const Vector3& eyeWorld, const Matrix& viewRow, const Matrix& projRow)
	{
		m_globalConstantBufferData.eyeWorld = eyeWorld;
		m_globalConstantBufferData.view = viewRow.Transpose();
		m_globalConstantBufferData.proj = projRow.Transpose();
		m_globalConstantBufferData.invProj = projRow.Invert().Transpose();
		m_globalConstantBufferData.viewProj = (viewRow * projRow).Transpose();
	}

	void SceneBase::UpdateGUI()
	{
		m_camera.UpdateGUI();
	}

	void SceneBase::Update(float dt)
	{
		m_camera.SetAspectRatio(m_pGraphics->GetD3D11()->GetAspectRatio());
		m_camera.UpdateViewDirection();
	}
}
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
		m_pCamera->UpdateGUI();
	}

	void SceneBase::Update(float dt)
	{
	}
}
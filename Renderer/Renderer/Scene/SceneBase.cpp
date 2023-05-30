#include "SceneBase.h"

#include <imgui.h>

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SceneBase::Initialize(Graphics* pGraphics)
	{
		m_pGraphics = pGraphics;
	};

	SceneBase::~SceneBase()
	{
	}
}
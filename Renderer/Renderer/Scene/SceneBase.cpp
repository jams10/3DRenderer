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
	};

	SceneBase::~SceneBase()
	{
	}
}
#pragma once

#include "Utility/CustomMacros.h"

#include <string>

namespace NS
{
	class Model;
	class GraphicsProcessor;

	class SkyBox
	{
	public:
		SkyBox();
		~SkyBox();
		void Initialize(GraphicsProcessor* const pGraphics, std::string textureFilePath);
		void Render(GraphicsProcessor* const pGraphics);

	private:
		Model* m_cube;
	};
}


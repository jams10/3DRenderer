#pragma once

#include "Utility/CustomMacros.h"

#include <string>

#include "Model/TextureResources.h"

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

		const TextureResourcesForGPU* GetTextureResource() const;

	private:
		Model* m_cube;
	};
}



#include "Skybox.h"

#include <vector>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Model/Model.h"
#include "Model/MeshGenerator.h"

namespace NS
{
	SkyBox::SkyBox()
	{
		m_cube = nullptr;
	}

	SkyBox::~SkyBox()
	{
		if (m_cube)
		{
			m_cube->Shutdown();
			delete m_cube;
			m_cube = nullptr;
		}
	}

	void SkyBox::Initialize(GraphicsProcessor* const pGraphics, std::string textureFilePath)
	{
		std::vector<MeshForCPU> newMesh = { MeshGenerator::MakeBox(100.f) };
		//std::vector<MeshForCPU> newMesh = { MeshGenerator::MakeSphere(100.f, 20, 20) };
		newMesh[0].textures.albedoTextureFilename = textureFilePath;
		newMesh[0].textures.bIsUsingCubeMap = true;
		m_cube = new Model();
		m_cube->InitializeWithDrawingNormal(pGraphics, newMesh);
		m_cube->Update(0.f, pGraphics);
	}

	void SkyBox::Render(GraphicsProcessor* const pGraphics)
	{
		m_cube->Render(pGraphics);
	}
}
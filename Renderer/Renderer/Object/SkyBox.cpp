
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
			delete m_cube;
			m_cube = nullptr;
		}
	}

	void SkyBox::Initialize(GraphicsProcessor* const pGraphics, std::string textureFilePath)
	{
		std::vector<MeshForCPU> newMesh = { MeshGenerator::MakeBox(100.f) };
		//std::vector<MeshForCPU> newMesh = { MeshGenerator::MakeSphere(100.f, 20, 20) };
		newMesh[0].textures.albedoTextureFilename = textureFilePath;
		newMesh[0].bIsSkybox = true;
		m_cube = new Model();
		m_cube->InitializeWithDrawingNormal(pGraphics, newMesh);
		m_cube->Update(0.f, pGraphics);
	}

	void SkyBox::Render(GraphicsProcessor* const pGraphics)
	{
		m_cube->Render(pGraphics);
	}

	const TextureResourcesForGPU* SkyBox::GetTextureResource() const
	{
		if (m_cube == nullptr)
		{
			// 스카이 박스의 텍스쳐 리소스를 요청 했으나, 스카이 박스의 모델 조차 만들어지지 않은 상태. 즉, 초기화가 이루어지지 않았음.
			// 스카이 박스를 먼저 생성하지 않고 모델 초기화 함수에 스카이 박스 텍스쳐 리소스를 넘긴 경우일 수 있음.
			// 스카이 박스가 초기화 되어야 큐브 모델이 만들어지면서 meshForGPU와 텍스쳐 리소스도 함께 만들어짐.
			ASSERT_CRASH(m_cube != nullptr);
			return nullptr;
		}
		else
		{
			return m_cube->GetTextureResourcesForGPU();
		}
	}
}
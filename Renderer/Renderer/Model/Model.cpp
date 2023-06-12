#include "Model.h"

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"

#include <imgui.h>

namespace NS
{
	Model::Model(GraphicsProcessor* const pGraphics, const std::string& basePath, const std::string& filename)
	{
		this->Initialize(pGraphics, basePath, filename);
	}

	Model::Model(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		this->Initialize(pGraphics, meshes);
	}

	void Model::Initialize(GraphicsProcessor* const pGraphics, const std::string& basePath, const std::string& filename)
	{
		// 파일로부터 로드.

	}

	void Model::Initialize(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		m_meshWorldTransformData.world = Matrix();
		m_meshWorldTransformData.worldInvTranspose = Matrix();

		// 상수 버퍼 리소스 생성.
		pGraphics->GetD3D11()->CreateConstantBuffer(m_meshWorldTransformData, m_meshVertexConstantBuffer);
		pGraphics->GetD3D11()->CreateConstantBuffer(m_materialData, m_meshPixelConstantBuffer);

		for (const MeshForCPU& meshForCPU : meshes)
		{
			auto newMeshForGPU = std::make_shared<MeshForGPU>();
			newMeshForGPU->indexCount = UINT(meshForCPU.indices.size());
			newMeshForGPU->vertexCount = UINT(meshForCPU.vertices.size());
			newMeshForGPU->stride = UINT(sizeof(Vertex));

			// 정점, 인덱스 버퍼 생성.
			pGraphics->GetD3D11()->CreateVertexBuffer(meshForCPU.vertices, newMeshForGPU->vertexBuffer);
			pGraphics->GetD3D11()->CreateIndexBuffer(meshForCPU.indices, newMeshForGPU->indexBuffer);

			// 텍스쳐 자원 생성.
			if (!meshForCPU.albedoTextureFilename.empty())
			{
				// TODO : 텍스쳐 자원 불러오는것 따로 빼기.
				pGraphics->GetD3D11()->CreateTexture(meshForCPU.albedoTextureFilename, newMeshForGPU->albedoTexture, newMeshForGPU->albedoSRV);
			}

			// 앞서 만들어준 상수 버퍼를 넣어줌.
			newMeshForGPU->vertexConstantBuffer = m_meshVertexConstantBuffer;
			newMeshForGPU->pixelConstantBuffer = m_meshPixelConstantBuffer;

			m_meshes.push_back(newMeshForGPU);
		}
	}

	void Model::Update(float dt, GraphicsProcessor* const pGraphics)
	{
		// 월드 변환 행렬. SRT 순서.
		m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_position);
		// 모델 트랜스폼 상수 버퍼 업데이트.
		UpdateModelTransformConstantBuffer(pGraphics);

		m_materialData.diffuse = Vector3(m_materialDiffuse);
		m_materialData.specular = Vector3(m_materialSpecular);

		// 모델 머티리얼 상수 버퍼 업데이트
		UpdateModelMaterialConstantBuffer(pGraphics);
	}

	void Model::UpdateGUI()
	{
		ImGui::Begin("Model");

		ImGui::Text("Transform");
		ImGui::SliderFloat3("Translation", &m_position.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scaling", &m_scale.x, 0.1f, 2.0f);

		if (m_bUseMaterial)
		{
			ImGui::Text("Material");
			if (m_bUseLightingFactor)
			{
				ImGui::SliderFloat("Diffuse", &m_materialDiffuse, 0.5f, 1.0f);
				ImGui::SliderFloat("Specular", &m_materialSpecular, 0.5f, 1.0f);
				ImGui::SliderFloat("Shineness", &m_materialData.shininess, 1.0f, 256.0f);
			}
			ImGui::Checkbox("Use Texture", &m_bUseTexture);
			if (!m_bUseTexture)
			{
				ImGui::ColorEdit3("Color", &m_materialData.color.x);
			}
		}
		ImGui::End();
	}

	void Model::UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics)
	{
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.world;
		m_meshWorldTransformData.worldInvTranspose.Translation(Vector3(0.0f));
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Invert().Transpose();
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Transpose();
		m_meshWorldTransformData.world = m_meshWorldTransformData.world.Transpose(); // 쉐이더에 넘겨주기 전, colum major로 만들어줌.

		// 정점 상수 버퍼 업데이트.
		pGraphics->GetD3D11()->UpdateBuffer(m_meshWorldTransformData, m_meshVertexConstantBuffer);
	}

	void Model::UpdateModelMaterialConstantBuffer(GraphicsProcessor* const pGraphics)
	{
		if (m_bUseTexture == true)
			m_materialData.bUseTexture = 1u;
		else
			m_materialData.bUseTexture = 0u;

		// 픽셀 상수 버퍼 업데이트
		pGraphics->GetD3D11()->UpdateBuffer(m_materialData, m_meshPixelConstantBuffer);
	}

	void Model::Render(GraphicsProcessor* const pGraphics)
	{
		for (const auto& meshForGPU : m_meshes)
		{
			pGraphics->GetD3D11()->Render(*meshForGPU.get());
		}
	}
	void Model::Shutdown()
	{
		size_t size = m_meshes.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_meshes[i]->Shutdown();
		}
	}
}
#include "Model.h"

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"

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
				pGraphics->GetD3D11()->CreateTexture(meshForCPU.albedoTextureFilename, newMeshForGPU->albedoTexture, newMeshForGPU->albedoSRV);
			}

			// 앞서 만들어준 상수 버퍼를 넣어줌. 모든 mesh들이 같은 상수 버퍼를 공용으로 사용함.
			newMeshForGPU->vertexConstantBuffer = m_meshVertexConstantBuffer;

			m_meshes.push_back(newMeshForGPU);
		}
	}

	//void Model::UpdatetTransform(GraphicsProcessor* const pGraphics, const Matrix& worldRowMatrix)
	//{
	//	m_meshWorldTransformData.world = worldRowMatrix.Transpose();

	//	m_meshWorldTransformData.worldInvTranspose = worldRowMatrix;
	//	m_meshWorldTransformData.worldInvTranspose.Translation(Vector3(0.0f));
	//	m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Invert().Transpose();
	//	m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Transpose();

	//	UpdateConstantBuffers(pGraphics);
	//}

	void Model::UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics)
	{
		m_meshWorldTransformData.world = m_meshWorldTransformData.world.Transpose(); // 쉐이더에 넘겨주기 전, colum major로 만들어줌.
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.world;
		m_meshWorldTransformData.worldInvTranspose.Translation(Vector3(0.0f));
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Invert().Transpose();
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Transpose();

		// 정점 상수 버퍼 업데이트.
		pGraphics->GetD3D11()->UpdateBuffer(m_meshWorldTransformData, m_meshVertexConstantBuffer);
	}

	void Model::Render(GraphicsProcessor* const pGraphics)
	{
		for (const auto& meshForGPU : m_meshes)
		{
			pGraphics->GetD3D11()->Render(*meshForGPU.get());
		}
	}
}
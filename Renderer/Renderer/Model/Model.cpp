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
		// ���Ϸκ��� �ε�.

	}

	void Model::Initialize(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		m_meshWorldTransformData.world = Matrix();
		m_meshWorldTransformData.worldInvTranspose = Matrix();

		// ��� ���� ���ҽ� ����.
		pGraphics->GetD3D11()->CreateConstantBuffer(m_meshWorldTransformData, m_meshVertexConstantBuffer);

		for (const MeshForCPU& meshForCPU : meshes)
		{
			auto newMeshForGPU = std::make_shared<MeshForGPU>();
			newMeshForGPU->indexCount = UINT(meshForCPU.indices.size());
			newMeshForGPU->vertexCount = UINT(meshForCPU.vertices.size());
			newMeshForGPU->stride = UINT(sizeof(Vertex));

			// ����, �ε��� ���� ����.
			pGraphics->GetD3D11()->CreateVertexBuffer(meshForCPU.vertices, newMeshForGPU->vertexBuffer);
			pGraphics->GetD3D11()->CreateIndexBuffer(meshForCPU.indices, newMeshForGPU->indexBuffer);

			// �ؽ��� �ڿ� ����.
			if (!meshForCPU.albedoTextureFilename.empty())
			{
				pGraphics->GetD3D11()->CreateTexture(meshForCPU.albedoTextureFilename, newMeshForGPU->albedoTexture, newMeshForGPU->albedoSRV);
			}

			// �ռ� ������� ��� ���۸� �־���. ��� mesh���� ���� ��� ���۸� �������� �����.
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
		m_meshWorldTransformData.world = m_meshWorldTransformData.world.Transpose(); // ���̴��� �Ѱ��ֱ� ��, colum major�� �������.
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.world;
		m_meshWorldTransformData.worldInvTranspose.Translation(Vector3(0.0f));
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Invert().Transpose();
		m_meshWorldTransformData.worldInvTranspose = m_meshWorldTransformData.worldInvTranspose.Transpose();

		// ���� ��� ���� ������Ʈ.
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
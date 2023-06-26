#include "Model.h"

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"

#include <imgui.h>

namespace NS
{
	Model::Model(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		this->Initialize(pGraphics, meshes);
	}

	void Model::Initialize(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		m_meshWorldTransformData.world = Matrix();
		m_meshWorldTransformData.worldInvTranspose = Matrix();

		// 상수 버퍼 리소스 생성.
		pGraphics->GetD3D11()->CreateConstantBuffer(m_meshWorldTransformData, m_meshVertexConstantBuffer);
		pGraphics->GetD3D11()->CreateConstantBuffer(m_materialConstantData, m_meshPixelConstantBuffer);

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
			if (!meshForCPU.textures.albedoTextureFilename.empty())
			{
				// TODO : 텍스쳐 자원 불러오는것 따로 빼기.
				pGraphics->GetD3D11()->CreateTexture(meshForCPU.textures.albedoTextureFilename, newMeshForGPU->textures.albedoTexture, newMeshForGPU->textures.albedoSRV);
			}

			// 앞서 만들어준 상수 버퍼를 넣어줌.
			newMeshForGPU->vertexConstantBuffer = m_meshVertexConstantBuffer;
			newMeshForGPU->pixelConstantBuffer = m_meshPixelConstantBuffer;

			m_meshes.push_back(newMeshForGPU);
		}
	}

	void Model::InitializeWithDrawingNormal(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes)
	{
		Initialize(pGraphics, meshes);

		// 상수 버퍼 리소스 생성.
		pGraphics->GetD3D11()->CreateConstantBuffer(m_meshNormalDrawingData, m_meshNormalConstantBuffer);

		int nMeshes = meshes.size();

		for (int i = 0; i < nMeshes; ++i)
		{
			MeshNormalForCPU newMeshNormal;
			int nVerts = meshes[i].vertices.size();
			for (int j = 0; j < nVerts; ++j)
			{
				VertexForDrawingNormal vn;
				vn.position = meshes[i].vertices[j].position;
				vn.normalModel = meshes[i].vertices[j].normalModel;
				vn.texcoord.x = 0; // normal의 시작점.
				newMeshNormal.vertices.push_back(vn);
				vn.texcoord.x = 1; // normal의 끝점.
				newMeshNormal.vertices.push_back(vn);

				newMeshNormal.indices.push_back(uint32_t(2 * j));
				newMeshNormal.indices.push_back(uint32_t(2 * j + 1));
			}

			auto newMeshNormalForGPU = std::make_shared<MeshNormalForGPU>();
			newMeshNormalForGPU->indexCount = UINT(newMeshNormal.indices.size());
			newMeshNormalForGPU->vertexCount = UINT(newMeshNormal.vertices.size());
			newMeshNormalForGPU->stride = UINT(sizeof(VertexForDrawingNormal));

			// 정점, 인덱스 버퍼 생성.
			pGraphics->GetD3D11()->CreateVertexBuffer(newMeshNormal.vertices, newMeshNormalForGPU->vertexBuffer);
			pGraphics->GetD3D11()->CreateIndexBuffer(newMeshNormal.indices, newMeshNormalForGPU->indexBuffer);

			// 앞서 만들어준 상수 버퍼를 넣어줌.
			newMeshNormalForGPU->vertexConstantBuffer = m_meshNormalConstantBuffer;

			m_normalMeshes.push_back(newMeshNormalForGPU);
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
		// 모델 노말 그리기 상수 버퍼 업데이트.
		UpdateModelNormalConstantBuffer(pGraphics);

		m_materialConstantData.diffuse = Vector3(m_materialDiffuse);
		m_materialConstantData.specular = Vector3(m_materialSpecular);

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
		ImGui::Checkbox("Draw normals", &m_bDrawNormals);
		if (m_bDrawNormals)
		{
			ImGui::SliderFloat("Normal Scale", &m_meshNormalDrawingData.scale, 0.5f, 2.0f);
		}

		if (m_bUseMaterial)
		{
			ImGui::Text("Material");
			if (m_bUseLightingFactor)
			{
				ImGui::SliderFloat("Diffuse", &m_materialDiffuse, 0.5f, 1.0f);
				ImGui::SliderFloat("Specular", &m_materialSpecular, 0.5f, 1.0f);
				ImGui::SliderFloat("Shineness", &m_materialConstantData.shininess, 1.0f, 256.0f);
			}
			ImGui::Checkbox("Use Texture", &m_bUseTexture);
			if (!m_bUseTexture)
			{
				ImGui::ColorEdit3("Color", &m_materialConstantData.color.x);
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

	void Model::UpdateModelNormalConstantBuffer(GraphicsProcessor* const pGraphics)
	{
		pGraphics->GetD3D11()->UpdateBuffer(m_meshNormalDrawingData, m_meshNormalConstantBuffer);
	}

	void Model::UpdateModelMaterialConstantBuffer(GraphicsProcessor* const pGraphics)
	{
		if (m_bUseTexture == true)
			m_materialConstantData.bUseTexture = 1u;
		else
			m_materialConstantData.bUseTexture = 0u;

		// 픽셀 상수 버퍼 업데이트
		pGraphics->GetD3D11()->UpdateBuffer(m_materialConstantData, m_meshPixelConstantBuffer);
	}

	void Model::Render(GraphicsProcessor* const pGraphics)
	{
		int size = m_meshes.size();
		vector<ID3D11Buffer*> constBuffersVS;
		vector<ID3D11Buffer*> constBuffersPS;
		vector<ID3D11ShaderResourceView*> shaderResources;

		if (size > 0)
		{
			constBuffersVS.push_back(m_meshes[0]->vertexConstantBuffer.Get());
			constBuffersPS.push_back(m_meshes[0]->pixelConstantBuffer.Get());
			shaderResources.push_back(m_meshes[0]->textures.albedoSRV.Get());
		}

		for (int i = 0; i < size; ++i)
		{
			pGraphics->GetD3D11()->Render(m_meshes[i]->vertexBuffer.Get(), m_meshes[i]->indexBuffer.Get(),
				constBuffersVS, constBuffersPS, shaderResources,
				m_meshes[i]->indexCount, m_meshes[i]->stride, m_meshes[i]->offset);
		}
	}

	void Model::RenderNormal(GraphicsProcessor* const pGraphics)
	{
		int size = m_meshes.size();
		vector<ID3D11Buffer*> constBuffersVS;
		vector<ID3D11Buffer*> constBuffersPS;
		vector<ID3D11ShaderResourceView*> shaderResources;

		if (size > 0)
		{
			constBuffersVS.push_back(m_meshes[0]->vertexConstantBuffer.Get());
		}

		if (m_normalMeshes.empty()) return;

		constBuffersVS.push_back(m_normalMeshes[0]->vertexConstantBuffer.Get());

		size = m_normalMeshes.size();
		// 노말 벡터 그리기.
		for (int i = 0; i < size; ++i)
		{
			pGraphics->GetD3D11()->Render(m_normalMeshes[i]->vertexBuffer.Get(), m_normalMeshes[i]->indexBuffer.Get(),
				constBuffersVS, constBuffersPS, shaderResources,
				m_normalMeshes[i]->indexCount, m_normalMeshes[i]->stride, m_normalMeshes[i]->offset);
		}
	}

	void Model::Shutdown()
	{
		size_t size = m_meshes.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_meshes[i]->Shutdown();
		}
		size = m_normalMeshes.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_normalMeshes[i]->Shutdown();
		}

		m_meshVertexConstantBuffer.Reset();
		m_meshNormalConstantBuffer.Reset();
		m_meshPixelConstantBuffer.Reset();
	}
}
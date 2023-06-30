#pragma once

#include "Utility/CustomMacros.h"

#include <string>
#include <vector>
#include <memory>

#include "MeshForCPU.h"
#include "MeshForGPU.h"
#include "ConstantData.h"
#include "TextureResources.h"

/*
*	<Model>
*	���� 3D ���� ��Ÿ���� Ŭ����.
*	���Ϸ� ���� Ȥ�� ���� ������ MeshForCPU ������ ������ ���ڷ� �޾� �������� �ʿ��� D3D11 ���ҽ����� �������.
*	D3D11 ���ҽ� ����, ���ε�, ������ �Լ� ȣ����� ó����.
*/

namespace NS
{
	class GraphicsProcessor;
	using DirectX::SimpleMath::Matrix;

	class Model
	{
	public:
		Model() {}
		~Model();
		Model(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes);

		void Initialize(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes);
		void InitializeWithDrawingNormal(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes);

		virtual void Update(float dt, GraphicsProcessor* const pGraphics);
		virtual void UpdateGUI();

		void Render(GraphicsProcessor* const pGraphics);
		void RenderNormal(GraphicsProcessor* const pGraphics);

	public:
		Vector3 m_position = Vector3(0.f, 0.f, 0.f);
		Vector3 m_rotation = Vector3(0.f);
		Vector3 m_scale = Vector3(1.f);

		MeshConstants m_meshWorldTransformData;         // �� ���� ��ȯ ����� ��� ��� ���� ������ ����ü.
		DrawingNormalConstants m_meshNormalDrawingData; // �޽� �븻�� �׸� �� ����� sclae ���� ��� ��� ���� ����ü.
		MaterialConstants m_materialConstantData;       // �����ÿ� ����� ���� ������.
		bool m_bUseMaterial = true;
		bool m_bUseLightingFactor = true;
		bool m_bUseTexture = false;
		bool m_bDrawNormals = false;
		
	protected:
		void UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics);
		void UpdateModelNormalConstantBuffer(GraphicsProcessor* const pGraphics);
		void UpdateModelMaterialConstantBuffer(GraphicsProcessor* const pGraphics);

		void CreateTextureResources(GraphicsProcessor* const pGraphics, const MeshForCPU& meshForCPU, MeshForGPU& newMeshForGPU);

		float m_materialDiffuse = 1.0f;
		float m_materialSpecular = 1.0f;
		float m_normalScale = 1.0f;

		std::vector<std::shared_ptr<MeshForGPU>> m_meshes;               // 3D �� ������ �ҷ����� �� �� ���� �ȿ� ���� ���� 3D �޽��� ������ �� ����.
		std::vector<std::shared_ptr<MeshNormalForGPU>> m_normalMeshes;   // �븻�� �׸��� ���� �޽� ������.

		// �𵨿� ��� ��� �޽�(�ϳ��� ��� ���� �޽�)�� ���������� ����� ���� ��� ����.
		ComPtr<ID3D11Buffer> m_meshVertexConstantBuffer; // ���� m_meshWorldTransformData�� ��� GPU�� �����ϴ� �뵵.
		ComPtr<ID3D11Buffer> m_meshNormalConstantBuffer; // �޽� �븻�� �׸� ���, �븻�� ���̸� �����ϴ� ���� ���� 
		ComPtr<ID3D11Buffer> m_meshPixelConstantBuffer;  // ���� m_materialData�� ��� GPU�� �����ϴ� �뵵.
	};
}



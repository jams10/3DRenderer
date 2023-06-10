#pragma once

#include "Utility/CustomMacros.h"

#include <string>
#include <vector>
#include <memory>

#include "MeshForCPU.h"
#include "MeshForGPU.h"
#include "ConstantData.h"

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
		Model(GraphicsProcessor* const pGraphics, const std::string& basePath, const std::string& filename);
		Model(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes);

		void Initialize(GraphicsProcessor* const pGraphics, const std::string& basePath, const std::string& filename);
		void Initialize(GraphicsProcessor* const pGraphics, const std::vector<MeshForCPU>& meshes);

		void Update(float dt, GraphicsProcessor* const pGraphics);
		void UpdateGUI();

		void Render(GraphicsProcessor* const pGraphics);
		void Shutdown();

	public:
		Vector3 m_position = Vector3(0.f, 0.f, 2.f);
		Vector3 m_rotation = Vector3(0.f);
		Vector3 m_scale = Vector3(1.f);

		MeshConstants m_meshWorldTransformData; // �� ���� ��ȯ ����� ��� ��� ���� ������ ����ü.
		MaterialConstants m_materialData;       // �����ÿ� ����� ���� ������.
		bool m_bUseTexture = false;
		
	private:
		void UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics);
		void UpdateModelMaterialConstantBuffer(GraphicsProcessor* const pGraphics);

		std::vector<std::shared_ptr<MeshForGPU>> m_meshes; // 3D �� ������ �ҷ����� �� �� ���� �ȿ� ���� ���� 3D �޽��� ������ �� ����.

		// �𵨿� ��� ��� �޽�(�ϳ��� ��� ���� �޽�)�� ���������� ����� ���� ��� ����.
		// ���� m_meshWorldTransformData�� ��� GPU�� �����ϴ� �뵵.
		ComPtr<ID3D11Buffer> m_meshVertexConstantBuffer; 
		ComPtr<ID3D11Buffer> m_meshPixelConstantBuffer;
	};
}



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

		//void UpdatetTransform(GraphicsProcessor* const pGraphics, const Matrix& worldRowMatrix);
		void UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics);

		void Render(GraphicsProcessor* const pGraphics);

	public:
		MeshConstants m_meshWorldTransformData; // �� ���� ��ȯ ����� ��� ��� ���� ������ ����ü.
		
	//private:
		std::vector<std::shared_ptr<MeshForGPU>> m_meshes; // 3D �� ������ �ҷ����� �� �� ���� �ȿ� ���� ���� 3D �޽��� ������ �� ����.

		// �𵨿� ��� ��� �޽�(�ϳ��� ��� ���� �޽�)�� ���������� ����� ���� ��� ����.
		// ���� m_meshWorldTransformData�� ��� GPU�� �����ϴ� �뵵.
		ComPtr<ID3D11Buffer> m_meshVertexConstantBuffer; 
	};
}



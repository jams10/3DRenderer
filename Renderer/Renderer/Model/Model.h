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
*	개별 3D 모델을 나타내는 클래스.
*	파일로 부터 혹은 직접 생성한 MeshForCPU 유형의 변수를 인자로 받아 렌더링에 필요한 D3D11 리소스들을 만들어줌.
*	D3D11 리소스 생성, 바인딩, 렌더링 함수 호출까지 처리함.
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

		MeshConstants m_meshWorldTransformData; // 모델 월드 변환 행렬이 담긴 상수 버퍼 데이터 구조체.
		MaterialConstants m_materialData;       // 라이팅에 사용할 재질 데이터.
		bool m_bUseTexture = false;
		
	private:
		void UpdateModelTransformConstantBuffer(GraphicsProcessor* const pGraphics);
		void UpdateModelMaterialConstantBuffer(GraphicsProcessor* const pGraphics);

		std::vector<std::shared_ptr<MeshForGPU>> m_meshes; // 3D 모델 파일을 불러왔을 때 한 파일 안에 여러 개의 3D 메쉬가 존재할 수 있음.

		// 모델에 담긴 모든 메쉬(하나일 경우 단일 메쉬)가 공통적으로 사용할 정점 상수 버퍼.
		// 위의 m_meshWorldTransformData를 담아 GPU에 전달하는 용도.
		ComPtr<ID3D11Buffer> m_meshVertexConstantBuffer; 
		ComPtr<ID3D11Buffer> m_meshPixelConstantBuffer;
	};
}



#pragma once

#include <vector>
#include <string>

#include "Utility/CustomMacros.h"
#include "Vertex.h"
#include "TextureResources.h"

/*
*	CPU 쪽에서 만들어줄 Mesh를 나타내는 구조체들을 정의함.
*	직접 정점들을 정의하거나 파일로부터 읽어서 정점, 인덱스 벡터를 채워주고 이를 활용해 GPU에서 사용할 정점 버퍼와 InputLayout을 생성함.
*/
namespace NS
{
	struct MeshForCPU
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		TextureResourcesForCPU textures;
		bool bIsSkybox = false;
	};

	struct MeshNormalForCPU
	{
		std::vector<VertexForDrawingNormal> vertices;
		std::vector<uint32_t> indices;
	};
}
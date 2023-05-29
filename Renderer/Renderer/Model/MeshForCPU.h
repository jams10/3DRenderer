#pragma once

#include <vector>
#include <string>

#include "Utility/CustomMacros.h"
#include "Vertex.h"

/*
*	CPU 쪽에서 만들어줄 Mesh를 나타내는 구조체들을 정의함.
*	직접 정점들을 정의하거나 파일로부터 읽어서 정점, 인덱스 벡터를 채워주고 이를 활용해 GPU에서 사용할 정점 버퍼와 InputLayout을 생성함.
*/
namespace NS
{
	struct MeshForCPUWithColorVertex
	{
		std::vector<ColorVertex> vertices;
		std::vector<uint16_t> indices;
		UINT indexCount;
	};

	struct MeshForCPUWithBasicVertex
	{
		std::vector<BasicVertex> vertices;
		std::vector<uint16_t> indices;
		UINT indexCount;
		std::string textureFilePath;
	};
}
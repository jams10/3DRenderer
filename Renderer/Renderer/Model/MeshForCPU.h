#pragma once

#include <vector>
#include <string>

#include "Utility/CustomMacros.h"
#include "Vertex.h"

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
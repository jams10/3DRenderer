#pragma once

#include <vector>

#include "Utility/CustomMacros.h"
#include "Vertex.h"

namespace NS
{
	struct MeshForCPU
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
		UINT indexCount;
	};
}
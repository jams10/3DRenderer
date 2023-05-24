#pragma once

#include <vector>

#include "Utility/CustomMacros.h"
#include "Vertex.h"

namespace NS
{
	struct MeshForCPU
	{
		std::vector<Vertex> m_vertices;
		std::vector<uint16_t> m_indices;
		UINT m_indexCount;
	};
}
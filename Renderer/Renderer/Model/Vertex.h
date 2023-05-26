#pragma once

#include <directxtk/SimpleMath.h>

#include "Utility/CustomMacros.h"

namespace NS
{
	using DirectX::SimpleMath::Vector3;

	struct Vertex
	{
		Vector3 position;
		Vector3 color;
	};
}
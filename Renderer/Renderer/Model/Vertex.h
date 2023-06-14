#pragma once

#include <directxtk/SimpleMath.h>

#include "Utility/CustomMacros.h"

/*
*	개별 정점들을 구조체로 정의함.
*	정점의 경우 기본적으로 위치를 가지지만, 추가적으로 색상, 노말, 텍스쳐 좌표등을 가질 수 있으므로 용도에 따라 다양한 정점들을 정의.
*/
namespace NS
{
	using DirectX::SimpleMath::Vector3;
	using DirectX::SimpleMath::Vector2;

	struct Vertex
	{
		Vector3 position;
		Vector3 normalModel;
		Vector2 texcoord;
		Vector3 tangentModel;
		Vector3 color;
	};

	struct VertexForDrawingNormal
	{
		Vector3 position;
		Vector3 normalModel;
		Vector2 texcoord;
	};
}
#pragma once

#include <directxtk/SimpleMath.h>

#include "Utility/CustomMacros.h"

/*
*	���� �������� ����ü�� ������.
*	������ ��� �⺻������ ��ġ�� ��������, �߰������� ����, �븻, �ؽ��� ��ǥ���� ���� �� �����Ƿ� �뵵�� ���� �پ��� �������� ����.
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
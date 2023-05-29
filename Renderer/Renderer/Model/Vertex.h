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

	struct ColorVertex
	{
		Vector3 position;
		Vector3 color;
	};

	struct BasicVertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texcoord;
	};
}
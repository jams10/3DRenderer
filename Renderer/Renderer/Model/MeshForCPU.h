#pragma once

#include <vector>
#include <string>

#include "Utility/CustomMacros.h"
#include "Vertex.h"

/*
*	CPU �ʿ��� ������� Mesh�� ��Ÿ���� ����ü���� ������.
*	���� �������� �����ϰų� ���Ϸκ��� �о ����, �ε��� ���͸� ä���ְ� �̸� Ȱ���� GPU���� ����� ���� ���ۿ� InputLayout�� ������.
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
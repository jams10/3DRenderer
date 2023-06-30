#pragma once

#include <vector>
#include <string>

#include "Utility/CustomMacros.h"
#include "Vertex.h"
#include "TextureResources.h"

/*
*	CPU �ʿ��� ������� Mesh�� ��Ÿ���� ����ü���� ������.
*	���� �������� �����ϰų� ���Ϸκ��� �о ����, �ε��� ���͸� ä���ְ� �̸� Ȱ���� GPU���� ����� ���� ���ۿ� InputLayout�� ������.
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
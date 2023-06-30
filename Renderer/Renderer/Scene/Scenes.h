#pragma once

#include "SCDrawTriangle.h"
#include "SCDrawTexturedCube.h"
#include "SCPhongShading.h"
#include "SCEnviromentMapping.h"
#include "Utility/CustomMacros.h"

/*
*	���� ������ ��� ������ ��Ƶΰ� ���� ��� ���Ϸ� include �� �� �ֵ��� ��.
*	���� ������ Ÿ���� �����ϴ� enum�� ���� ���� ������ �� �ֵ��� ��.
*/
namespace NS
{
	enum SceneType
	{
		DRAW_TRIANGLE,
		DRAW_TEXTUREDCUBE,
		PHONG_SHADING,
		ENVIROMENT_MAPPING,
	};
}
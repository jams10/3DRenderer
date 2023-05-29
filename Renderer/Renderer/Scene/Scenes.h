#pragma once

#include "SCDrawTriangle.h"
#include "SCDrawTexturedCube.h"
#include "Utility/CustomMacros.h"

/*
*	여러 씬들의 헤더 파일을 모아두고 단일 헤더 파일로 include 할 수 있도록 함.
*	여러 씬들의 타입을 정의하는 enum을 통해 씬을 구별할 수 있도록 함.
*/
namespace NS
{
	enum SceneType
	{
		DRAW_TRIANGLE,
		DRAW_TEXTUREDCUBE,
	};
}
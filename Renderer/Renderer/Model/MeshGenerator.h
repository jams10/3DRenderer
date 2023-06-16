#pragma once

#include "Utility/CustomMacros.h"

#include "MeshForCPU.h"

namespace NS
{
	class MeshGenerator
	{
    public:
        static MeshForCPU MakeTriangle(const float scale = 1.0f);

        static MeshForCPU MakeSquare(const float scale = 1.0f, const Vector2 texScale = Vector2(1.0f));

        static MeshForCPU MakeSquareGrid(const int numSlices, const int numStacks,
            const float scale = 1.0f, const Vector2 texScale = Vector2(1.0f));

        static MeshForCPU MakeBox(const float scale = 1.0f);

        static MeshForCPU MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);

        static MeshForCPU MakeSphere(const float radius, const int numSlices, const int numStacks,
            const Vector2 texScale = Vector2(1.0f));
	};

}


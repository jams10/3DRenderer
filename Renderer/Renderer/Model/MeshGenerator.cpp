#include "MeshGenerator.h"

#include <vector>

namespace NS
{
    using std::vector;
    using DirectX::SimpleMath::Matrix;

    MeshForCPU MeshGenerator::MakeTriangle(const float scale)
    {
        MeshForCPU newMesh;
        vector<Vector3> positions;
        vector<Vector3> normalModel;
        vector<Vector2> texcoord;
        vector<Vector3> tangentModel;
        vector<Vector3> colors;

        positions.push_back(Vector3(-1.f, -1.f, 0.f) * scale);
        positions.push_back(Vector3(0.f, 1.f, 0.f) * scale);
        positions.push_back(Vector3(1.f, -1.f, 0.f) * scale);
        normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        texcoord.push_back(Vector2(0.0f, 1.0f));
        texcoord.push_back(Vector2(0.5f, 0.0f));
        texcoord.push_back(Vector2(0.1f, 1.0f));
        tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

        for (int i = 0; i < 3; ++i)
        {
            Vertex v;
            v.position = positions[i];
            v.normalModel = normalModel[i];
            v.texcoord = texcoord[i];
            v.tangentModel = tangentModel[i];
            v.color = colors[i];
            newMesh.vertices.push_back(v);
        }

        newMesh.indices = { 0, 1, 2, };
        return newMesh;
    }

	MeshForCPU MeshGenerator::MakeSquare(const float scale, const Vector2 texScale)
	{
        vector<Vector3> positions;
        vector<Vector3> colors;
        vector<Vector3> normals;
        vector<Vector2> texcoords;

        positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        MeshForCPU newMesh;

        for (size_t i = 0; i < positions.size(); i++)
        {
            Vertex v;
            v.position = positions[i];
            v.normalModel = normals[i];
            v.texcoord = texcoords[i] * texScale;
            v.tangentModel = Vector3(1.0f, 0.0f, 0.0f);
            v.color = colors[i];

            newMesh.vertices.push_back(v);
        }

        newMesh.indices = { 0, 1, 2, 0, 2, 3,};

        return newMesh;
	}

	MeshForCPU MeshGenerator::MakeSquareGrid(const int numSlices, const int numStacks, const float scale, const Vector2 texScale)
	{
        MeshForCPU newMesh;

        float dx = 2.0f / numSlices;
        float dy = 2.0f / numStacks;

        float y = 1.0f;
        for (int j = 0; j < numStacks + 1; j++) 
        {
            float x = -1.0f;
            for (int i = 0; i < numSlices + 1; i++) 
            {
                Vertex v;
                v.position = Vector3(x, y, 0.0f) * scale;
                v.normalModel = Vector3(0.0f, 0.0f, -1.0f);
                v.texcoord = Vector2(x + 1.0f, y + 1.0f) * 0.5f * texScale;
                v.tangentModel = Vector3(1.0f, 0.0f, 0.0f);
                v.color = Vector3(0.0f, 0.0f, 1.0f);

                newMesh.vertices.push_back(v);

                x += dx;
            }
            y -= dy;
        }

        for (int j = 0; j < numStacks; j++) 
        {
            for (int i = 0; i < numSlices; i++) 
            {
                newMesh.indices.push_back((numSlices + 1) * j + i);
                newMesh.indices.push_back((numSlices + 1) * j + i + 1);
                newMesh.indices.push_back((numSlices + 1) * (j + 1) + i);
                newMesh.indices.push_back((numSlices + 1) * (j + 1) + i);
                newMesh.indices.push_back((numSlices + 1) * j + i + 1);
                newMesh.indices.push_back((numSlices + 1) * (j + 1) + i + 1);
            }
        }

        return newMesh;
	}

	MeshForCPU MeshGenerator::MakeBox(const float scale)
	{
        vector<Vector3> positions;
        vector<Vector3> colors;
        vector<Vector3> normals;
        vector<Vector2> texcoords;

        // 윗면
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 아랫면
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 앞면
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 뒷면
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 왼쪽
        positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        // 오른쪽
        positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
        texcoords.push_back(Vector2(0.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 0.0f));
        texcoords.push_back(Vector2(1.0f, 1.0f));
        texcoords.push_back(Vector2(0.0f, 1.0f));

        MeshForCPU newMesh;
        for (size_t i = 0; i < positions.size(); i++) 
        {
            Vertex v;
            v.position = positions[i];
            v.normalModel = normals[i];
            v.texcoord = texcoords[i];
            v.color = colors[i];
            newMesh.vertices.push_back(v);
        }

        newMesh.indices = {
            0,  1,  2,  0,  2,  3,  // 윗면
            4,  5,  6,  4,  6,  7,  // 아랫면
            8,  9,  10, 8,  10, 11, // 앞면
            12, 13, 14, 12, 14, 15, // 뒷면
            16, 17, 18, 16, 18, 19, // 왼쪽
            20, 21, 22, 20, 22, 23  // 오른쪽
        };

        return newMesh;
	}

	MeshForCPU MeshGenerator::MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices)
	{
        // Texture 좌표계때문에 (numSlices + 1) x 2 개의 버텍스 사용

        const float dTheta = -DirectX::XM_2PI / float(numSlices);

        MeshForCPU newMesh;

        vector<Vertex>& vertices = newMesh.vertices;

        // 옆면의 바닥 버텍스들 (인덱스 0 이상 numSlices 미만)
        for (int i = 0; i <= numSlices; i++) 
        {
            Vertex v;
            v.position = Vector3::Transform(Vector3(bottomRadius, -0.5f * height, 0.0f),
                                            Matrix::CreateRotationY(dTheta * float(i)));
            v.normalModel = v.position - Vector3(0.0f, -0.5f * height, 0.0f);
            v.normalModel.Normalize();
            v.texcoord = Vector2(float(i) / numSlices, 1.0f);

            vertices.push_back(v);
        }

        // 옆면의 맨 위 버텍스들 (인덱스 numSlices 이상 2 * numSlices 미만)
        for (int i = 0; i <= numSlices; i++) 
        {
            Vertex v;
            v.position = Vector3::Transform(Vector3(topRadius, 0.5f * height, 0.0f),
                                            Matrix::CreateRotationY(dTheta * float(i)));
            v.normalModel = v.position - Vector3(0.0f, 0.5f * height, 0.0f);
            v.normalModel.Normalize();
            v.texcoord = Vector2(float(i) / numSlices, 0.0f);

            vertices.push_back(v);
        }

        vector<uint32_t>& indices = newMesh.indices;

        for (int i = 0; i < numSlices; i++) 
        {
            indices.push_back(i);
            indices.push_back(i + numSlices + 1);
            indices.push_back(i + 1 + numSlices + 1);

            indices.push_back(i);
            indices.push_back(i + 1 + numSlices + 1);
            indices.push_back(i + 1);
        }

        return newMesh;
	}

	MeshForCPU MeshGenerator::MakeSphere(const float radius, const int numSlices, const int numStacks, const Vector2 texScale)
	{
        // Texture 좌표계때문에 (numSlices + 1) 개의 버텍스 사용 (마지막에 닫아주는
        // 버텍스가 중복) Stack은 y 위쪽 방향으로 쌓아가는 방식

        const float dTheta = -DirectX::XM_2PI / float(numSlices);
        const float dPhi = -DirectX::XM_PI / float(numStacks);

        MeshForCPU newMesh;

        vector<Vertex>& vertices = newMesh.vertices;

        for (int j = 0; j <= numStacks; j++)
        {
            // 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
            Vector3 stackStartPoint = Vector3::Transform(Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

            for (int i = 0; i <= numSlices; i++) 
            {
                Vertex v;

                // 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
                v.position = Vector3::Transform(stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

                v.normalModel = v.position; // 원점이 구의 중심
                v.normalModel.Normalize();
                v.texcoord = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks) * texScale;

                // texcoord가 위로 갈수록 증가
                Vector3 biTangent = Vector3(0.0f, 1.0f, 0.0f);

                Vector3 normalOrth = v.normalModel - biTangent.Dot(v.normalModel) * v.normalModel;
                normalOrth.Normalize();

                v.tangentModel = biTangent.Cross(normalOrth);
                v.tangentModel.Normalize();

                vertices.push_back(v);
            }
        }

        vector<uint32_t>& indices = newMesh.indices;

        for (int j = 0; j < numStacks; j++) 
        {
            const int offset = (numSlices + 1) * j;
            for (int i = 0; i < numSlices; i++) 
            {
                indices.push_back(offset + i);
                indices.push_back(offset + i + numSlices + 1);
                indices.push_back(offset + i + 1 + numSlices + 1);

                indices.push_back(offset + i);
                indices.push_back(offset + i + 1 + numSlices + 1);
                indices.push_back(offset + i + 1);
            }
        }

        return newMesh;
	}

    // 입력 모델 모양이 구체라고 가정함.
    MeshForCPU MeshGenerator::SubdivideToSphere(const float radius, MeshForCPU meshForCPU)
    {
        // 구 모양으로 Subdivision을 진행하기 때문에 정점의 위치를 구의 표면으로 옮겨주는 람다 함수.
        auto ProjectVertex = [&](Vertex& v) 
        {
            v.normalModel = v.position;
            v.normalModel.Normalize();
            v.position = v.normalModel * radius;
        };

        // 원점이 중심이라고 가정하고 기존 정점들을 구체의 반지름 위치로 옮겨줌.
        for (auto& v : meshForCPU.vertices)
        {
            ProjectVertex(v);
        }

        // 기존 정점과 새로 추가된 정점들로 구성된 면(face) 단위로 노말 벡터를 업데이트 해주는 람다 함수.
        auto UpdateFaceNormal = [](Vertex& v0, Vertex& v1, Vertex& v2) 
        {
            auto faceNormal = (v1.position - v0.position).Cross(v2.position - v0.position);
            faceNormal.Normalize();
            v0.normalModel = faceNormal;
            v1.normalModel = faceNormal;
            v2.normalModel = faceNormal;
        };

        // 버텍스가 중복되는 구조로 구현
        MeshForCPU newMesh;
        uint32_t count = 0;
        for (size_t i = 0; i < meshForCPU.indices.size(); i += 3)
        {
            size_t i0 = meshForCPU.indices[i];
            size_t i1 = meshForCPU.indices[i + 1];
            size_t i2 = meshForCPU.indices[i + 2];

            Vertex v0 = meshForCPU.vertices[i0];
            Vertex v1 = meshForCPU.vertices[i1];
            Vertex v2 = meshForCPU.vertices[i2];

            Vertex v3; // v0, v2 정점 중간에 새 정점을 추가.
            v3.position = (v0.position + v2.position) * 0.5f;
            v3.texcoord = (v0.texcoord + v2.texcoord) * 0.5f;
            ProjectVertex(v3);

            Vertex v4; // v0, v1 정점 중간에 새 정점을 추가.
            v4.position = (v0.position + v1.position) * 0.5f;
            v4.texcoord = (v0.texcoord + v1.texcoord) * 0.5f;
            ProjectVertex(v4);

            Vertex v5; // v1, v2 정점 중간에 새 정점을 추가.
            v5.position = (v1.position + v2.position) * 0.5f;
            v5.texcoord = (v1.texcoord + v2.texcoord) * 0.5f;
            ProjectVertex(v5);

            //UpdateFaceNormal(v4, v1, v5);
            //UpdateFaceNormal(v0, v4, v3);
            //UpdateFaceNormal(v3, v4, v5);
            //UpdateFaceNormal(v3, v5, v2);

            newMesh.vertices.push_back(v4);
            newMesh.vertices.push_back(v1);
            newMesh.vertices.push_back(v5);

            newMesh.vertices.push_back(v0);
            newMesh.vertices.push_back(v4);
            newMesh.vertices.push_back(v3);

            newMesh.vertices.push_back(v3);
            newMesh.vertices.push_back(v4);
            newMesh.vertices.push_back(v5);

            newMesh.vertices.push_back(v3);
            newMesh.vertices.push_back(v5);
            newMesh.vertices.push_back(v2);

            // 인덱스 채우기.
            for (uint32_t j = 0; j < 12; j++) 
            {
                newMesh.indices.push_back(j + count);
            }
            count += 12;
        }

        return newMesh;
    }
}
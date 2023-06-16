#include "MeshGenerator.h"

#include <vector>

namespace NS
{
    using std::vector;
    using DirectX::SimpleMath::Matrix;

    MeshForCPU MeshGenerator::MakeTriangle(const float scale)
    {
        MeshForCPU meshForCPU;
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
            meshForCPU.vertices.push_back(v);
        }

        meshForCPU.indices = { 0, 1, 2, };
        return meshForCPU;
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

        // ����
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

        // �Ʒ���
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

        // �ո�
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

        // �޸�
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

        // ����
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

        // ������
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
            0,  1,  2,  0,  2,  3,  // ����
            4,  5,  6,  4,  6,  7,  // �Ʒ���
            8,  9,  10, 8,  10, 11, // �ո�
            12, 13, 14, 12, 14, 15, // �޸�
            16, 17, 18, 16, 18, 19, // ����
            20, 21, 22, 20, 22, 23  // ������
        };

        return newMesh;
	}

	MeshForCPU MeshGenerator::MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices)
	{
        // Texture ��ǥ�趧���� (numSlices + 1) x 2 ���� ���ؽ� ���

        const float dTheta = -DirectX::XM_2PI / float(numSlices);

        MeshForCPU newMesh;

        vector<Vertex>& vertices = newMesh.vertices;

        // ������ �ٴ� ���ؽ��� (�ε��� 0 �̻� numSlices �̸�)
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

        // ������ �� �� ���ؽ��� (�ε��� numSlices �̻� 2 * numSlices �̸�)
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

        vector<uint16_t>& indices = newMesh.indices;

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
        // Texture ��ǥ�趧���� (numSlices + 1) ���� ���ؽ� ��� (�������� �ݾ��ִ�
        // ���ؽ��� �ߺ�) Stack�� y ���� �������� �׾ư��� ���

        const float dTheta = -DirectX::XM_2PI / float(numSlices);
        const float dPhi = -DirectX::XM_PI / float(numStacks);

        MeshForCPU newMesh;

        vector<Vertex>& vertices = newMesh.vertices;

        for (int j = 0; j <= numStacks; j++)
        {
            // ���ÿ� ���� ���� �������� x-y ��鿡�� ȸ�� ���Ѽ� ���� �ø��� ����
            Vector3 stackStartPoint = Vector3::Transform(Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

            for (int i = 0; i <= numSlices; i++) 
            {
                Vertex v;

                // �������� x-z ��鿡�� ȸ����Ű�鼭 ���� ����� ����
                v.position = Vector3::Transform(stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

                v.normalModel = v.position; // ������ ���� �߽�
                v.normalModel.Normalize();
                v.texcoord = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks) * texScale;

                // texcoord�� ���� ������ ����
                Vector3 biTangent = Vector3(0.0f, 1.0f, 0.0f);

                Vector3 normalOrth = v.normalModel - biTangent.Dot(v.normalModel) * v.normalModel;
                normalOrth.Normalize();

                v.tangentModel = biTangent.Cross(normalOrth);
                v.tangentModel.Normalize();

                vertices.push_back(v);
            }
        }

        vector<uint16_t>& indices = newMesh.indices;

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
}

#include "ModelLoader.h"

#include <iostream>
#include <filesystem>

namespace NS
{
    using namespace DirectX::SimpleMath;

    void ModelLoader::Load(std::string basePath, std::string filename) 
    {
        this->basePath = basePath;

        Assimp::Importer importer;

        const aiScene* pScene = importer.ReadFile(
            this->basePath + filename,
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded); // �ﰢ������ ����, �޼� ��ǥ��� �� ������ �о��.

        Matrix matrix; // �ʱ� Ʈ������.
        ProcessNode(pScene->mRootNode, pScene, matrix);
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix matrix) 
    {
        std::cout << "\n<<LoadModel>>\n";
        std::cout << "Path : " << basePath << '\n';
        std::cout << node->mName.C_Str() << " : " << node->mNumMeshes << " "
                  << node->mNumChildren << '\n\n';

        // �޽��� Ʈ������ ����� ����.
        Matrix m;
        ai_real* temp = &node->mTransformation.a1;
        float* mTemp = &m._11;
        for (int t = 0; t < 16; t++) 
        {
            mTemp[t] = float(temp[t]);
        }
        m = m.Transpose() * matrix;

        for (UINT i = 0; i < node->mNumMeshes; i++) 
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // ���� �ִ� �޽����� ������.
            auto newMesh = this->ProcessMesh(mesh, scene);   // ������ �޽��� ó��.

            for (auto& v : newMesh.vertices) 
            { 
                // �޽� �������� ��ġ�� �⺻ Ʈ������ ����� ���� ��ȯ.
                v.position = DirectX::SimpleMath::Vector3::Transform(v.position, m);
            }

            meshes.push_back(newMesh); // MeshForCPU �����̳ʿ� �־���.
        }

        for (UINT i = 0; i < node->mNumChildren; i++) 
        {
            this->ProcessNode(node->mChildren[i], scene, m); // �ش� ����� �ڽ� ���鵵 ProcessNode�� ���� ���� �����͸� ����.
        }
    }

    MeshForCPU ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) 
    {
        // ä���� ����, �ε��� ����.
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // ���ڷ� �Ѿ�� mesh�� �������� ��ȸ�ϸ鼭 ���� �о �츮�� MeshForCPU�� �� ���������� ä����.
        for (UINT i = 0; i < mesh->mNumVertices; i++) 
        { 
            Vertex vertex;

            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            vertex.normalModel.x = mesh->mNormals[i].x;
            vertex.normalModel.y = mesh->mNormals[i].y;
            vertex.normalModel.z = mesh->mNormals[i].z;
            vertex.normalModel.Normalize();

            if (mesh->mTextureCoords[0]) 
            {
                vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
                vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
            }

            vertices.push_back(vertex);
        }

        for (UINT i = 0; i < mesh->mNumFaces; i++) // �鿡 ����, ���� �̷�� �������� �ε����� �־���.
        { 
            aiFace face = mesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        MeshForCPU newMesh; // MeshForCPU �����̳ʿ� �־��� �� MeshForCPU ��ü ����. ������ �������� ����, �ε��� ���͸� �־���.
        newMesh.vertices = vertices;
        newMesh.indices = indices;

        // http://assimp.sourceforge.net/lib_html/materials.html
        if (mesh->mMaterialIndex >= 0) 
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // ������ �ش� �޽��� ���׸����� ������.

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)  // ���׸����� diffuse �ؽ��İ� �����ϸ� �ؽ����� ���� �̸��� ������.
            {    
                aiString filepath;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

                std::string fullPath =
                    this->basePath +
                    std::string(std::filesystem::path(filepath.C_Str())
                        .filename()
                        .string()); // assimp�� ���� ������ �ؽ��� ���� �̸��� �⺻ ���� ��θ� ���ļ� MeshForCPU ��ü�� �ؽ��� �����̸��� ��������.

                newMesh.textures.albedoTextureFilename = fullPath;
            }
        }

        return newMesh;
    }
}

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
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded); // 삼각형으로 구성, 왼손 좌표계로 모델 파일을 읽어옴.

        Matrix matrix; // 초기 트랜스폼.
        ProcessNode(pScene->mRootNode, pScene, matrix);
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix matrix) 
    {
        std::cout << "\n<<LoadModel>>\n";
        std::cout << "Path : " << basePath << '\n';
        std::cout << node->mName.C_Str() << " : " << node->mNumMeshes << " "
                  << node->mNumChildren << '\n\n';

        // 메쉬의 트랜스폼 행렬을 얻어옴.
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
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // 씬에 있는 메쉬들을 가져옴.
            auto newMesh = this->ProcessMesh(mesh, scene);   // 가져온 메쉬를 처리.

            for (auto& v : newMesh.vertices) 
            { 
                // 메쉬 정점들의 위치를 기본 트랜스폼 행렬을 통해 변환.
                v.position = DirectX::SimpleMath::Vector3::Transform(v.position, m);
            }

            meshes.push_back(newMesh); // MeshForCPU 컨테이너에 넣어줌.
        }

        for (UINT i = 0; i < node->mNumChildren; i++) 
        {
            this->ProcessNode(node->mChildren[i], scene, m); // 해당 노드의 자식 노드들도 ProcessNode를 통해 정점 데이터를 추출.
        }
    }

    MeshForCPU ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) 
    {
        // 채워줄 정점, 인덱스 벡터.
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // 인자로 넘어온 mesh의 정점들을 순회하면서 값을 읽어서 우리의 MeshForCPU에 들어갈 정점들을에 채워줌.
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

        for (UINT i = 0; i < mesh->mNumFaces; i++) // 면에 접근, 면을 이루는 정점들의 인덱스를 넣어줌.
        { 
            aiFace face = mesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        MeshForCPU newMesh; // MeshForCPU 컨테이너에 넣어줄 새 MeshForCPU 개체 생성. 위에서 추출해준 정점, 인덱스 벡터를 넣어줌.
        newMesh.vertices = vertices;
        newMesh.indices = indices;

        // http://assimp.sourceforge.net/lib_html/materials.html
        if (mesh->mMaterialIndex >= 0) 
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // 씬에서 해당 메쉬의 머테리얼을 가져옴.

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)  // 머테리얼의 diffuse 텍스쳐가 존재하면 텍스쳐의 파일 이름을 가져옴.
            {    
                aiString filepath;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

                std::string fullPath =
                    this->basePath +
                    std::string(std::filesystem::path(filepath.C_Str())
                        .filename()
                        .string()); // assimp를 통해 가져온 텍스쳐 파일 이름과 기본 파일 경로를 합쳐서 MeshForCPU 개체의 텍스쳐 파일이름에 저장해줌.

                newMesh.textures.albedoTextureFilename = fullPath;
            }
        }

        return newMesh;
    }
}
#pragma once

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <string>
#include <vector>

#include "CustomMacros.h"
#include "Model/MeshForCPU.h"

namespace NS
{
    class ModelLoader 
    {
    public:
        void Load(std::string basePath, std::string filename);

        void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix matrix);

        MeshForCPU ProcessMesh(aiMesh* mesh, const aiScene* scene);

    public:
        std::string basePath;           // ���� ���.
        std::vector<MeshForCPU> meshes; // ���Ϸκ��� ������ ���� ������ ���� �����̳�.
    };
}



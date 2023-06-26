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
        std::string basePath;           // 파일 경로.
        std::vector<MeshForCPU> meshes; // 파일로부터 추출한 정점 정보를 담을 컨테이너.
    };
}



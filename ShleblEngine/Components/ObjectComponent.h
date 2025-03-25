#pragma once

#include <string>
#include "BaseComponent.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

class KatamariBall;

class ObjectComponent : public BaseComponent
{
protected:
    const std::string fNameModel;
    //void ProcessNode(aiNode* node, const aiScene* scene);
    //void ProcessMesh(aiMesh* mesh, const aiScene* scene);
public:
    bool isPickedUp;
    KatamariBall* kb;
    ObjectComponent(Game* g, std::string fileNameModel, const wchar_t* fileNameTexture);
    ~ObjectComponent() override = default;
    void Initialize() override;
    //void SetPosition(DirectX::SimpleMath::Vector3 p);
    //void SetRotation(DirectX::SimpleMath::Quaternion q);
    //void SetScale(DirectX::SimpleMath::Vector3 s);
};

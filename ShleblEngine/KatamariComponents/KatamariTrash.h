#pragma once
#include "../Components/ObjectComponent.h"

class KatamariTrash : public ObjectComponent
{
public:
    bool isPickedUp;
    KatamariTrash(Game* g, std::string fileNameModel, const wchar_t* fileNameTexture, float katSize, DirectX::SimpleMath::Vector3 collOffset);
    DirectX::BoundingSphere collision;
    DirectX::SimpleMath::Vector3 initRelPos;
    DirectX::SimpleMath::Quaternion invKbRot;
    DirectX::SimpleMath::Vector3 originCollisionOffset;
    float gameSize;
    void Update() override;
    void SetPosition(DirectX::SimpleMath::Vector3 p) override;
};
#pragma once

#include "../Components/SphereComponent.h"

class KatamariBallOutline;
class KatamariGame;

class KatamariBall : public SphereComponent
{
protected:
    KatamariGame* kGame;
    void UpdateSize(float absorbedSize);
public:
    float rotationDrag;
    float rotationMaxSpeed;
    float moveMaxSpeed;
    float moveDrag;
    float radius = 1.0;
    DirectX::SimpleMath::Quaternion savedRot;
    DirectX::SimpleMath::Vector3 velocity;
    DirectX::BoundingSphere collision;
    float gameSize;
    KatamariBall(Game* game);
    ~KatamariBall() override;
    void Initialize() override;
    void Draw() override;
    void Update() override;
    void DestroyResources() override;
    void SetDirection(DirectX::SimpleMath::Vector3 dir);
    void SetPosition(DirectX::SimpleMath::Vector3 p) override;
};
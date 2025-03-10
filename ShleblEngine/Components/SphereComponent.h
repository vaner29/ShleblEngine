#pragma once
#include "BaseComponent.h"
#include "../Dependencies/SimpleMath.h"
class SphereComponent :
    public BaseComponent
{
public:
    struct Point
    {
        DirectX::SimpleMath::Vector4 pos;
        DirectX::SimpleMath::Vector4 col;
    };
protected:
    ID3D11InputLayout* layout;
    ID3D11Buffer* const_buffer_;
    UINT strides[1];
    UINT offsets[1];
    std::vector<Point> points_;
    std::vector<int> indices_;
public:
    DirectX::SimpleMath::Quaternion rotation;
    DirectX::SimpleMath::Vector3 position;
    SphereComponent(Game* game, float radius, int sliceCount, int stackCount, DirectX::SimpleMath::Vector4 col1, DirectX::SimpleMath::Vector4 col2);
    void Draw() override;
    void Initialize() override;
    void Update() override;
};

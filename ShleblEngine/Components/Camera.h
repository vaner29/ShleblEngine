#pragma once
#include "../Dependencies/SimpleMath.h"

class Camera
{
protected:
    DirectX::SimpleMath::Matrix viewProj;
public:
    bool IsOrthographic;
    float OrthographicWidth;
    float OrthographicHeight;
    float FOV;
    float AspectRatio;
    float NearPlane;
    float FarPlane;
    DirectX::SimpleMath::Vector3 Position;
    DirectX::SimpleMath::Vector3 Target;
    DirectX::SimpleMath::Vector3 Up;
    Camera();
    void UpdateMatrix();
    DirectX::SimpleMath::Matrix GetMatrix() const;
};

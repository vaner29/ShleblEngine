#pragma once
#include "../Dependencies/SimpleMath.h"
#include <vector>

class Camera
{
protected:
    DirectX::SimpleMath::Matrix viewProj;
public:
    DirectX::SimpleMath::Matrix view_;
    DirectX::SimpleMath::Matrix proj_;
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
    DirectX::SimpleMath::Matrix GetViewMatrix() const;
    DirectX::SimpleMath::Matrix GetViewProj() const;
    DirectX::SimpleMath::Matrix GetView() const;
    DirectX::SimpleMath::Matrix GetProj() const;
    std::vector<DirectX::SimpleMath::Vector4> GetFrustumCornersWorldSpace(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
};

#include "Camera.h"
#include "../Dependencies/SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

Camera::Camera()
{
    viewProj = Matrix::Identity;

    FOV = XM_PI / 4.0f;
    AspectRatio = 1.0f;
    NearPlane = 1.0f;
    FarPlane = 1000.f;
    Position = Vector3::Backward * 10.0f;
    Target = Vector3::Zero;
    Up = Vector3::Up;
    IsOrthographic = false;
    OrthographicWidth = 50.0f;
    OrthographicHeight = 50.0f;
}

void Camera::UpdateMatrix()
{
    if (IsOrthographic)
        viewProj = Matrix::CreateLookAt(Position, Target, Up) * Matrix::CreateOrthographic(OrthographicWidth, OrthographicHeight, NearPlane / 10.0f, FarPlane);
    else
        viewProj = Matrix::CreateLookAt(Position, Target, Up) * Matrix::CreatePerspectiveFieldOfView(FOV, AspectRatio, NearPlane, FarPlane);
}

Matrix Camera::GetMatrix() const
{
    return viewProj;
}

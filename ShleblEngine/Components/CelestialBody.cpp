#include "CelestialBody.h"

#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

constexpr int SliceCount = 32;
constexpr int StackCount = 32;

CelestialBody::CelestialBody(Game* game, const CelestialBodyDesc& desc) :
    SphereComponent(game, desc.Radius, SliceCount, StackCount, desc.PrimaryColor, desc.SecondaryColor),
    spinAxis(desc.SpinAxis), spinSpeed(desc.SpinSpeed), orbitAnchor(desc.OrbitAnchor), orbitRadius(desc.OrbitRadius),
    orbitOrientationForward(desc.OrbitOrientationForward), orbitOrientationUp(desc.OrbitOrientationUp), orbitSpeed(desc.OrbitSpeed)
{
    if (orbitAnchor)
    {
        relativePosition = desc.OrbitOrientationForward * orbitRadius;
        position = orbitAnchor->position + relativePosition;
    }
    else
    {
        position = desc.OrbitOrientationForward * orbitRadius;
    }
}

void CelestialBody::Update()
{
    if (orbitAnchor)
    {
        relativePosition = XMVector4Transform(relativePosition, Matrix::CreateFromAxisAngle(orbitOrientationUp, game->delta_time_ * orbitSpeed));
        position = orbitAnchor->position + relativePosition;
    }
    else
    {
        position = XMVector4Transform(position, Matrix::CreateFromAxisAngle(orbitOrientationUp, game->delta_time_ * orbitSpeed));
    }
    rotation *= Quaternion::CreateFromAxisAngle(spinAxis, spinSpeed * game->delta_time_);
    SphereComponent::Update();
}

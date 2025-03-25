//#include "CelestialBody.h"
//
//#include "Game.h"
//
//using namespace DirectX;
//using namespace SimpleMath;
//
//constexpr int SliceCount = 32;
//constexpr int StackCount = 32;
//
//CelestialBody::CelestialBody(Game* game, const CelestialBodyDesc& desc) :
//    SphereComponent(game, desc.Radius, SliceCount, StackCount, desc.PrimaryColor, desc.SecondaryColor),
//    spinAxis(desc.SpinAxis), spinSpeed(desc.SpinSpeed), orbitAnchor(desc.OrbitAnchor), orbitRadius(desc.OrbitRadius),
//    orbitOrientationForward(desc.OrbitOrientationForward), orbitOrientationUp(desc.OrbitOrientationUp), orbitSpeed(desc.OrbitSpeed), absoluteVelocity(Vector3::Zero)
//{
//    if (orbitAnchor)
//    {
//        relativePosition = desc.OrbitOrientationForward * orbitRadius;
//        position = orbitAnchor->position + relativePosition;
//    }
//    else
//    {
//        position = desc.OrbitOrientationForward * orbitRadius;
//    }
//}
//
//void CelestialBody::Update()
//{
//    /*if (orbitAnchor)
//    {
//        relativePosition = XMVector4Transform(relativePosition, Matrix::CreateFromAxisAngle(orbitOrientationUp, game->delta_time_ * orbitSpeed));
//        position = orbitAnchor->position + relativePosition;
//    }
//    else
//    {
//        position = XMVector4Transform(position, Matrix::CreateFromAxisAngle(orbitOrientationUp, game->delta_time_ * orbitSpeed));
//    }*/
//    rotation *= Quaternion::CreateFromAxisAngle(spinAxis, spinSpeed * game->delta_time_);
//
//    const float accelerationRate = 0.05f; // Tune this: how fast they accelerate
//    if (absoluteVelocity.LengthSquared() > 0.01f) // Avoid amplifying tiny velocities
//    {
//        Vector3 velocityDirection = absoluteVelocity;
//        velocityDirection.Normalize();
//        absoluteVelocity += velocityDirection * accelerationRate * game->delta_time_;
//    }
//
//    // Update position with velocity
//    position += absoluteVelocity * game->delta_time_;
//    SphereComponent::Update();
//
//}

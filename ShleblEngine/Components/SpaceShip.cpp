#include "SpaceShip.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

SpaceShip::SpaceShip(Game* game) : CelestialBody(game, []() {
    CelestialBodyDesc desc;
    desc.Radius = 1.0f; // Custom radius
    desc.SpinSpeed = 0.0f; // Custom spin speed
    desc.OrbitAnchor = nullptr; // Custom orbit anchor (nullptr in this case)
    //desc.OrbitOrientationForward = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // Custom forward orientation
    //desc.OrbitOrientationUp = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f); // Custom up orientation
    desc.OrbitRadius = 100.0f; // Custom orbit radius
    desc.OrbitSpeed = 10.0f; // Custom orbit speed
    desc.PrimaryColor = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f); // Custom primary color (red)
    desc.SecondaryColor = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f); // Custom secondary color (blue)
    return desc;
    }()) {
    // Additional SpaceShip initialization code can go here
}

void SpaceShip::Update()
{
    SphereComponent::Update();
}

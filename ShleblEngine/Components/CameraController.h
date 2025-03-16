#pragma once
#include "InputDevice.h"
#include "CelestialBody.h"

class Game;
class SpaceGame;

class CameraController
{
public:
    SpaceGame* sGame;
    CelestialBody* targetBody;
    DirectX::SimpleMath::Vector3 up;
    DirectX::SimpleMath::Vector3 relativePos;
    Game* game;
    bool OrbitMode;
    bool followShip = false;

    float yaw;
    float pitch;
    float speed;

    explicit CameraController(Game* g);
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
};

#pragma once
#include "InputDevice.h"
//#include "GameComponents/CelestialBodyComponent.h"

class Game;
class SpaceGame;

class CameraController
{
public:
    SpaceGame* sGame;
    //CelestialBodyComponent* targetBody;
    DirectX::SimpleMath::Vector3 up;
    DirectX::SimpleMath::Vector3 relativePos;
    Game* game;
    bool OrbitMode;
    // fps camera
    float yaw;
    float pitch;
    float speed;
    // end of fps camera
    explicit CameraController(Game* g);
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
};

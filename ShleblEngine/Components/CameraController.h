#pragma once
#include "InputDevice.h"
#include "GameComponent.h"
#include "../KatamariComponents/KatamariBall.h"

class Game;
class SpaceGame;
class KatamariGame;

class CameraController
{
public:
    SpaceGame* sGame;
    KatamariGame* kGame;
    //GameComponent* targetBody;
    KatamariBall* targetBall;
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

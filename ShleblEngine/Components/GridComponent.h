#pragma once
#include "BaseComponent.h"


class Game;

class GridComponent : public BaseComponent
{
public:
    GridComponent(Game* game, float cellSize, int lineCount);
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetRotation(DirectX::SimpleMath::Quaternion q) override { rotation = q; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};
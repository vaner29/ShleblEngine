#pragma once
#include <vector>
#include "BaseComponent.h"
#include "../Dependencies/SimpleMath.h"

class Game;

class GridComponent :
    public BaseComponent
{
public:
    struct Point
    {
        DirectX::SimpleMath::Vector4 pos;
        DirectX::SimpleMath::Vector4 col;
    };
protected:
    //ID3DBlob* vertexShaderByteCode;
    ID3D11Buffer* constBuffer;
    std::vector<Point> points;
    std::vector<int> indices;
public:
    GridComponent(Game* game, float cellSize, int lineCount);
    void Draw() override;
    void Initialize() override;
    void Update() override;
};

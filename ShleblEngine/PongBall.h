#pragma once
#include "RectangleComponent.h"
#include "SimpleMath.h"
class PongGame;

class PongBall:
    public RectangleComponent
{
private:
    PongGame* p_game_;
    void GenRndDirection();
public:
    DirectX::XMFLOAT4 center_ = DirectX::XMFLOAT4(0, 0, 0, 1.0f);
    float speed_;
    float radius_;
    float density_;
    DirectX::SimpleMath::Vector2 direction_;
    PongBall(Game* g);
    //PongBall(Game* g, float client_radius, float client_density);
    PongBall(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices);
    PongBall(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices, DirectX::XMFLOAT2 client_offset);
    void Update() override;
    void Reset();
    //std::vector<DirectX::XMFLOAT4> GeneratePoints(float radius, float density);
    //std::vector<int> GenerateIndices(float density);
    void SetPosition(DirectX::XMFLOAT2 pos);
    void SetY(float y);
    void SetX(float x);
    float GetX() const;
    float GetY() const;
    DirectX::XMFLOAT2 GetPosition() const;
};

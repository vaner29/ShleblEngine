#pragma once
#include "BaseComponent.h"
class RectangleComponent :
    public BaseComponent
{
private:
	ID3D11Buffer* const_buffer_;
public:
	DirectX::XMFLOAT4 offset_;
	RectangleComponent(Game* g);
	RectangleComponent(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices);
	RectangleComponent(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices, DirectX::XMFLOAT2 client_offset);
	//void SetPosition(float x, float y);
	void SetPosition(DirectX::XMFLOAT2 pos);
	void SetY(float y);
	void SetX(float x);
	float GetX() const;
	float GetY() const;
	DirectX::XMFLOAT2 GetPosition() const;
	void Update() override;
	void Draw() override;
	void Initialize() override;
};


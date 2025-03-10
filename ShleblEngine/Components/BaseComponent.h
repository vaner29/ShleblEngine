#pragma once
#include "GameComponent.h"
#include <vector>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class BaseComponent : public GameComponent
{
private:
	/*DirectX::XMFLOAT4 points_[6] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};*/

protected:
	ID3D11InputLayout* layout_;
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	UINT strides[1];
	UINT offsets[1];
	std::vector<DirectX::XMFLOAT4> points_ = {
	DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	};
	//int indices_[3] = {0, 1, 2};
	std::vector<int> indices_ = { 0, 1, 2 };
public:
	BaseComponent(Game* g);
	BaseComponent(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices);
	~BaseComponent();
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
};
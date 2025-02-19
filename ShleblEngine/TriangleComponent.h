#pragma once
#include "GameComponent.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class TriangleComponent : public GameComponent
{
private:
	ID3D11InputLayout* layout_;
	DirectX::XMFLOAT4 points_[6] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};
	int indices_[3] = {0, 1, 2};
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	UINT strides[1];
	UINT offsets[1];
public:
	TriangleComponent(Game* g);
	TriangleComponent(Game* g, DirectX::XMFLOAT4 client_points[6]);
	~TriangleComponent();
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
};
#pragma once
#include "GameComponent.h"
#include <vector>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Vertex.h"

class Game;

class BaseComponent : public GameComponent
{
public:
	struct PointLightData {
		DirectX::SimpleMath::Vector4 position; // xyz = position, w = 1.0
		DirectX::SimpleMath::Vector4 color;    // RGB = color, A = intensity
	};

	struct CBDataPerObject
	{
		DirectX::SimpleMath::Matrix worldViewProj;
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix WorldView;
		DirectX::SimpleMath::Matrix invTrWorld;
		float isSpinningFloor;
		DirectX::SimpleMath::Vector3 diffuseColor;
		DirectX::SimpleMath::Vector3 specularColor;
		float shininess;
		PointLightData pointLights[10]; // Up to 4 closest lights
		int numPointLights;            // Number of active lights (0-4)
		float padding[3];              // Align to 256 bytes
	};
	struct CbDataCascade
	{
		DirectX::SimpleMath::Matrix ViewProj[5];
		DirectX::SimpleMath::Vector4 Distance;
	};
protected:
	ID3D11InputLayout* layout_;
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	UINT strides[1];
	UINT offsets[1];
	std::vector<Vertex> points_{};
	std::vector<UINT> indices_ = { 0, 1, 2 };
	D3D_PRIMITIVE_TOPOLOGY topologyType;
	bool isShadowCasting_;
public:
	ID3D11RasterizerState* rastState_;
	ID3D11RasterizerState* shadowRastState_;
	ID3D11SamplerState* samplerState_;
	ID3D11SamplerState* depthSamplerState_;
	ID3D11Buffer* objConstantBuffer = nullptr;
	ID3D11Buffer* cascadeConstantBuffer = nullptr;
	bool passThroughVS;
	bool colorModePS;
	const wchar_t* textureFileName_;
	BaseComponent(Game* g);
	BaseComponent(Game* g, std::vector<Vertex> client_points, std::vector<UINT> client_indices);
	~BaseComponent();
	void PrepareFrame() override;
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	float isSpinningFloor = 0.0f;
	DirectX::SimpleMath::Vector3 diffuseColor = { 1.0f, 1.0f, 1.0f };  // Default white
	DirectX::SimpleMath::Vector3 specularColor = { 1.0f, 1.0f, 1.0f }; // Default white
	float shininess = 32.0f; // Default shininess
};
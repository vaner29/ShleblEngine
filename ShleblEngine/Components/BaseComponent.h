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
		DirectX::SimpleMath::Matrix invTrWorld;
		DirectX::SimpleMath::Matrix world;
		float isSpinningFloor;
		DirectX::SimpleMath::Vector3 diffuseColor;
		DirectX::SimpleMath::Vector3 specularColor;
		float shininess;
		PointLightData pointLights[10]; // Up to 4 closest lights
		int numPointLights;            // Number of active lights (0-4)
		float padding[3];              // Align to 256 bytes
	};
	struct CBDataPerScene
	{
		//DirectX::SimpleMath::Vector4 lightPos;    // Point light position (x, y, z, w=1.0)
		//DirectX::SimpleMath::Vector4 lightColor;  // Light color (RGB) and intensity (A)
		DirectX::SimpleMath::Vector4 ambientStrength; // Ambient strength (xyz unused, w = strength)
		DirectX::SimpleMath::Vector4 viewPos;     // Camera position in world space
		//float radius;                             // Light radius (attenuation distance)
		float gTime;                              // Game time for animations
		float padding[3];                         // Align to 16-byte boundary
	};
protected:
	ID3D11InputLayout* layout_;
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	UINT strides[1];
	UINT offsets[1];
	//std::vector<DirectX::XMFLOAT4> points_ = {
	//DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	//};
	std::vector<Vertex> points_{};
	std::vector<UINT> indices_ = { 0, 1, 2 };
	D3D_PRIMITIVE_TOPOLOGY topologyType;
public:
	ID3D11Buffer** const_buffers_;
	bool passThroughVS;
	bool colorModePS;
	const wchar_t* textureFileName_;
	BaseComponent(Game* g);
	BaseComponent(Game* g, std::vector<Vertex> client_points, std::vector<UINT> client_indices);
	~BaseComponent();
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	float isSpinningFloor = 0.0f;
	DirectX::SimpleMath::Vector3 diffuseColor = { 1.0f, 1.0f, 1.0f };  // Default white
	DirectX::SimpleMath::Vector3 specularColor = { 1.0f, 1.0f, 1.0f }; // Default white
	float shininess = 32.0f; // Default shininess
};
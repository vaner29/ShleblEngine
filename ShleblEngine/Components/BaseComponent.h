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
	struct CBDataPerObject
	{
		DirectX::SimpleMath::Matrix worldViewProj;
		DirectX::SimpleMath::Matrix invTrWorld;
		float isSpinningFloor;
		//float padding[3]; // Align to 64 bytes
		DirectX::SimpleMath::Vector3 diffuseColor;
		DirectX::SimpleMath::Vector3 specularColor;
		float shininess;
	};
	struct CBDataPerScene
	{
		//DirectX::SimpleMath::Vector4 lightPos;
		//DirectX::SimpleMath::Vector4 lightColorAmbStr;
		//DirectX::SimpleMath::Vector4 viewDirSpecStr;
		DirectX::SimpleMath::Vector4 lightDir;
		DirectX::SimpleMath::Vector4 lightColor;
		DirectX::SimpleMath::Vector4 ambientStrength;
		DirectX::SimpleMath::Vector4 viewPos;
		float gTime;
		float padding[3]; // Align to 64 bytes
	};
protected:
	ID3D11InputLayout* layout_;
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	ID3D11Buffer** const_buffers_;
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
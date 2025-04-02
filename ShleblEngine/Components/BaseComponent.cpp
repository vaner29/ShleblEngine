#include "BaseComponent.h"
#include "Game.h"
#include "../Dependencies/DDSTextureLoader.h"
#include "../Tools/DataProcesser.h"

using namespace DirectX;
using namespace SimpleMath;

BaseComponent::BaseComponent(Game* g) : GameComponent(g), layout_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr),
	const_buffers_(new ID3D11Buffer* [2]), strides{}, offsets{}, passThroughVS(false), colorModePS(false),
	topologyType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), textureFileName_(L"Textures/wood.dds")
{

}

BaseComponent::BaseComponent(Game* g, std::vector<Vertex> client_points, std::vector<UINT> client_indices) : GameComponent(g)
{
	indices_ = client_indices;
	points_ = client_points;
}


BaseComponent::~BaseComponent()
{
}

void BaseComponent::DestroyResources()
{
	layout_->Release();
	vertex_buffer_->Release();
	index_buffer_->Release();
}

void BaseComponent::Draw()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(game->display_->client_width_);
	viewport.Height = static_cast<float>(game->display_->client_height_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context_->RSSetViewports(1, &viewport);

	game->context_->IASetInputLayout(layout_);
	game->context_->IASetPrimitiveTopology(topologyType);
	game->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	game->context_->IASetVertexBuffers(0, 1, &vertex_buffer_, strides, offsets);
	game->context_->VSSetConstantBuffers(0, 2, const_buffers_);
	game->context_->PSSetConstantBuffers(0, 2, const_buffers_);
	ID3D11ShaderResourceView* test = DataProcesser::GetTextureView(textureFileName_);
	game->context_->PSSetShaderResources(0, 1, &test);

	game->context_->DrawIndexed(indices_.size(), 0, 0);
}

void BaseComponent::Initialize()
{
	D3D_SHADER_MACRO ShaderMacros[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	int macrosCount = 0;
	if (passThroughVS)
	{
		ShaderMacros[macrosCount++] = { "VERTEX_PASS_THROUGH", "1" };
	}

	if (colorModePS)
	{
		ShaderMacros[macrosCount++] = { "TREAT_TEX_AS_COL", "1" };
	}

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game->device_->CreateInputLayout(
		inputElements,
		3,
		DataProcesser::GetVertexShaderBC("base")->GetBufferPointer(),
		DataProcesser::GetVertexShaderBC("base")->GetBufferSize(),
		&layout_);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * std::size(points_);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points_.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device_->CreateBuffer(&vertexBufDesc, &vertexData, &vertex_buffer_);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(UINT) * std::size(indices_);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices_.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device_->CreateBuffer(&indexBufDesc, &indexData, &index_buffer_);

	strides[0] = sizeof(Vertex);
	offsets[0] = 0;

	D3D11_BUFFER_DESC constBufPerObjDesc = {};
	constBufPerObjDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufPerObjDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufPerObjDesc.CPUAccessFlags = 0;
	constBufPerObjDesc.MiscFlags = 0;
	constBufPerObjDesc.StructureByteStride = 0;
	constBufPerObjDesc.ByteWidth = sizeof(CBDataPerObject);

	game->device_->CreateBuffer(&constBufPerObjDesc, nullptr, &const_buffers_[0]);

	D3D11_BUFFER_DESC constBufPerSceneDesc = {};
	constBufPerSceneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufPerSceneDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufPerSceneDesc.CPUAccessFlags = 0;
	constBufPerSceneDesc.MiscFlags = 0;
	constBufPerSceneDesc.StructureByteStride = 0;
	constBufPerSceneDesc.ByteWidth = sizeof(CBDataPerScene);

	game->device_->CreateBuffer(&constBufPerSceneDesc, nullptr, &const_buffers_[1]);

	/*auto res = CreateDDSTextureFromFile(game->Device.Get(), textureFileName, &diffuseTextureBuffer, &diffuseTextureView);
	game->Context->GenerateMips(diffuseTextureView);*/

	/*D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto res = game->device_->CreateSamplerState(&samplerStateDesc, &samplerState);

	const CD3D11_RASTERIZER_DESC rastDesc = CreateRasterizerStateDesc();

	res = game->device_->CreateRasterizerState(&rastDesc, &rastState);*/

}

void BaseComponent::Update()
{
	rotation.Normalize();
	const Matrix world = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);

	CBDataPerObject objData = {};
	objData.worldViewProj = world * game->Camera->GetMatrix();
	objData.invTrWorld = (Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation)).Invert().Transpose();
	objData.isSpinningFloor = this->isSpinningFloor;
	objData.diffuseColor = this->diffuseColor;
	objData.specularColor = this->specularColor;
	objData.shininess = this->shininess;

	CBDataPerScene sceneData = {};
	//sceneData.lightPos = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	//sceneData.lightColorAmbStr = Vector4(1.0f, 1.0f, 1.0f, 0.4f);
	//sceneData.viewDirSpecStr = Vector4(game->Camera->Position.x - game->Camera->Target.x, game->Camera->Position.y - game->Camera->Target.y, game->Camera->Position.z - game->Camera->Target.z, 0.0f);
	//sceneData.viewDirSpecStr.Normalize();
	//sceneData.viewDirSpecStr.w = 0.5f;
	//sceneData.lightPos.Normalize();
	sceneData.lightDir = Vector4(1.0f, -1.0f, 0.0f, 0.0f); // Example: light from top-right
	sceneData.lightColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // White light, full intensity
	sceneData.ambientStrength = Vector4(0.0f, 0.0f, 0.0f, 0.2f); // 20% ambient
	sceneData.viewPos = Vector4(game->Camera->Position.x, game->Camera->Position.y, game->Camera->Position.z, 1.0f);
	sceneData.gTime = game->totalest_time_; // Use Game’s total_time_

	game->context_->UpdateSubresource(const_buffers_[0], 0, nullptr, &objData, 0, 0);
	game->context_->UpdateSubresource(const_buffers_[1], 0, nullptr, &sceneData, 0, 0);
}

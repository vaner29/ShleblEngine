#include "BaseComponent.h"
#include "Game.h"
#include "../Dependencies/DDSTextureLoader.h"
#include "../Tools/DataProcesser.h"
#include "../KatamariComponents/KatamariGame.h"

using namespace DirectX;
using namespace SimpleMath;

CD3D11_RASTERIZER_DESC BaseComponent::CreateRasterizerStateDesc()
{
	CD3D11_RASTERIZER_DESC rastDesc = {};

	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	return rastDesc;
}

BaseComponent::BaseComponent(Game* g) : GameComponent(g), layout_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr),
strides{}, offsets{}, passThroughVS(false), colorModePS(false), rastState_(nullptr), samplerState_(nullptr),
topologyType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), textureFileName_(L"Textures/wood.dds"), isShadowCasting_(true)
{

}

BaseComponent::BaseComponent(Game* g, std::vector<Vertex> client_points, std::vector<UINT> client_indices) : GameComponent(g)
{
	indices_ = client_indices;
	points_ = client_points;
}

void BaseComponent::Initialize()
{
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

	game->device_->CreateBuffer(&constBufPerObjDesc, nullptr, &objConstantBuffer);

	D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto res = game->device_->CreateSamplerState(&samplerStateDesc, &samplerState_);

	D3D11_SAMPLER_DESC depthSamplerStateDesc = {};
	depthSamplerStateDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	depthSamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
	depthSamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.BorderColor[0] = 1.0f;
	depthSamplerStateDesc.BorderColor[1] = 1.0f;
	depthSamplerStateDesc.BorderColor[2] = 1.0f;
	depthSamplerStateDesc.BorderColor[3] = 1.0f;

	res = game->device_->CreateSamplerState(&depthSamplerStateDesc, &depthSamplerState_);

	const CD3D11_RASTERIZER_DESC rastDesc = CreateRasterizerStateDesc();

	res = game->device_->CreateRasterizerState(&rastDesc, &rastState_);

	CD3D11_RASTERIZER_DESC shadowRastDesc = CreateRasterizerStateDesc();

	shadowRastDesc.CullMode = D3D11_CULL_FRONT;
	shadowRastDesc.DepthClipEnable = false;

	res = game->device_->CreateRasterizerState(&shadowRastDesc, &shadowRastState_);
}


BaseComponent::~BaseComponent()
{
}

void BaseComponent::PrepareFrame()
{
	if (!isShadowCasting_)
		return;

	D3D11_VIEWPORT viewport;
	viewport.Width = 2048.0f;
	viewport.Height = 2048.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context_->RSSetViewports(1, &viewport);

	game->context_->IASetInputLayout(layout_);
	game->context_->IASetPrimitiveTopology(topologyType);
	game->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	game->context_->IASetVertexBuffers(0, 1, &vertex_buffer_, strides, offsets);

	game->context_->VSSetShader(DataProcesser::GetVertexShader("csm"), nullptr, 0);
	game->context_->VSSetConstantBuffers(0, 1, &objConstantBuffer);

	game->context_->PSSetShader(nullptr, nullptr, 0);
	game->context_->GSSetShader(DataProcesser::GetGeometryShader("csm"), nullptr, 0);
	game->context_->GSSetConstantBuffers(0, 1, game->GetCascadeCb());

	game->context_->DrawIndexed(indices_.size(), 0, 0);
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
	game->context_->VSSetShader(DataProcesser::GetVertexShader("gbuffer"), nullptr, 0); //TODO: move that too
	game->context_->VSSetConstantBuffers(0, 1, &objConstantBuffer);
	game->context_->PSSetShader(DataProcesser::GetPixelShader("gbuffer"), nullptr, 0);


	ID3D11ShaderResourceView* texture = DataProcesser::GetTextureView(textureFileName_);
	game->context_->PSSetShaderResources(0, 1, &texture);

	game->context_->PSSetSamplers(0, 1, &samplerState_);

	game->context_->DrawIndexed(indices_.size(), 0, 0);
}


void BaseComponent::Update()
{
	rotation.Normalize();
	const Matrix world = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);

	CBDataPerObject objData = {};
	objData.worldViewProj = world * game->Camera->GetViewProj();
	objData.world = world;
	objData.WorldView = world * game->Camera->GetView();
	objData.invTrWorld = world.Invert().Transpose();
	//objData.isSpinningFloor = isSpinningFloor;
	//objData.diffuseColor = diffuseColor;
	//objData.specularColor = specularColor;
	//objData.shininess = shininess;

	//CbDataCascade cascadeData = {};
	//auto tmp = game->GetDLight()->GetLightSpaceMatrices();
	//for (int i = 0; i < 5; ++i)
	//{
	//	cascadeData.ViewProj[i] = tmp[i];
	//}
	//cascadeData.Distance = game->GetDLight()->GetShadowCascadeDistances();

	//objData.WorldViewProj = world * cascadeData.ViewProj[0];

	game->context_->UpdateSubresource(objConstantBuffer, 0, nullptr, &objData, 0, 0);
	//game->context_->UpdateSubresource(cascadeConstantBuffer, 0, nullptr, &cascadeData, 0, 0);

}

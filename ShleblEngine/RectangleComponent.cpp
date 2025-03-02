#include "RectangleComponent.h"
#include "Game.h"

RectangleComponent::RectangleComponent(Game* g) : BaseComponent(g, { // TODO: fix
		DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	}, {0,1,2, 0,1,3}), offset_()
{
}

RectangleComponent::RectangleComponent(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices) : BaseComponent(g, client_points, client_indices), offset_()
{
}

RectangleComponent::RectangleComponent(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices, DirectX::XMFLOAT2 client_offset) : BaseComponent(g, client_points, client_indices)
{
	offset_.x = client_offset.x;
	offset_.y = client_offset.y;
}

void RectangleComponent::SetPosition(DirectX::XMFLOAT2 pos)
{
	offset_.x = pos.x;
	offset_.y = pos.y;
}

void RectangleComponent::SetY(float y)
{
	offset_.y = y;
}

void RectangleComponent::SetX(float x)
{
	offset_.x = x;
}

float RectangleComponent::GetX() const
{
	return offset_.x;
}

float RectangleComponent::GetY() const
{
	return offset_.y;
}

DirectX::XMFLOAT2 RectangleComponent::GetPosition() const
{
	return DirectX::XMFLOAT2(offset_.x, offset_.y);
}

void RectangleComponent::Update()
{
	game->context_->UpdateSubresource(const_buffer_, 0, 0, &offset_, 0, 0);
}

void RectangleComponent::Draw()
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
	game->context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	game->context_->IASetVertexBuffers(0, 1, &vertex_buffer_, strides, offsets);
	game->context_->VSSetConstantBuffers(0, 1, &const_buffer_);
	game->context_->DrawIndexed(std::size(indices_), 0, 0);
}

void RectangleComponent::Initialize()
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
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game->device_->CreateInputLayout(
		inputElements,
		2,
		game->vertex_shader_byte_code_->GetBufferPointer(),
		game->vertex_shader_byte_code_->GetBufferSize(),
		&layout_);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points_);

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
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indices_);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices_.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device_->CreateBuffer(&indexBufDesc, &indexData, &index_buffer_);

	strides[0] = 32;
	offsets[0] = 0;

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);

	game->device_->CreateBuffer(&constBufDesc, nullptr, &const_buffer_);

}
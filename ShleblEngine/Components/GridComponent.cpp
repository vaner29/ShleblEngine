#include "GridComponent.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

GridComponent::GridComponent(Game* g, float cellSize, int lineCount)
	: BaseComponent(g)
{
	constexpr Vector4 color = Vector4(0.15f, 0.15f, 0.15f, 1.0f);
	constexpr Vector4 boldColor = Vector4(0.3f, 0.3f, 0.3f, 1.0f);

	int k = 0;
	for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
	{
		points.push_back(Point({ Vector4(static_cast<float>(i), 0.0f, -cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color }));
		indices.push_back(k++);
		points.push_back(Point({ Vector4(static_cast<float>(i), 0.0f, cellSize * lineCount / 2, 1.0f), i % 5 == 0 ? boldColor : color }));
		indices.push_back(k++);
	}
	for (int i = 0 - lineCount / 2; i < lineCount / 2; ++i)
	{
		points.push_back(Point({ Vector4(-cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color }));
		indices.push_back(k++);
		points.push_back(Point({ Vector4(cellSize * lineCount / 2, 0.0f, static_cast<float>(i), 1.0f), i % 5 == 0 ? boldColor : color }));
		indices.push_back(k++);
	}
}

void GridComponent::Initialize()
{

	constexpr D3D11_INPUT_ELEMENT_DESC inputElements[] = {
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
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(points) * 2;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device_->CreateBuffer(&vertexBufDesc, &vertexData, &vertex_buffer_);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indices);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
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
	constBufDesc.ByteWidth = sizeof(SimpleMath::Matrix);

	game->device_->CreateBuffer(&constBufDesc, nullptr, &constBuffer);

}

void GridComponent::Update()
{
	Matrix worldViewProj = game->Camera->GetMatrix();
	worldViewProj = worldViewProj.Transpose();
	game->context_->UpdateSubresource(constBuffer, 0, nullptr, &worldViewProj, 0, 0);
}

void GridComponent::Draw()
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
	game->context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	game->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	game->context_->IASetVertexBuffers(0, 1, &vertex_buffer_, strides, offsets);
	game->context_->VSSetConstantBuffers(0, 1, &constBuffer);

	game->context_->DrawIndexed(indices.size(), 0, 0);
}

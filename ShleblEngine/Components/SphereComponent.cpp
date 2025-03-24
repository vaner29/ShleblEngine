#include "SphereComponent.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

SphereComponent::SphereComponent(Game* g, float radius, int sliceCount, int stackCount, Vector4 col1, Vector4 col2)
	: BaseComponent(g), rotation(Quaternion::Identity), position(Vector3::Zero)
{
	radius_ = radius;
	Point topPoint({ Vector4(0.0f, radius, 0.0f, 1.0f), col1 });
	Vector4::Lerp(col1, col2, 0.5f, topPoint.col);
	Point bottomPoint({ Vector4(0.0f, -radius, 0.0f, 1.0f), col1 });
	Vector4::Lerp(col1, col2, 0.5f, bottomPoint.col);

	points_.push_back(topPoint);

	const float phiStep = XM_PI / static_cast<float>(stackCount);
	const float thetaStep = XM_2PI / static_cast<float>(sliceCount);

	for (int i = 1; i <= stackCount - 1; ++i)
	{
		const float phi = static_cast<float>(i) * phiStep;

		for (int j = 0; j <= sliceCount; ++j)
		{
			const float theta = static_cast<float>(j) * thetaStep;
			Point p;
			p.pos.x = radius * sinf(phi) * cosf(theta);
			p.pos.y = radius * cosf(phi);
			p.pos.z = radius * sinf(phi) * sinf(theta);
			p.pos.w = 1.0f;
			Vector4::Lerp(col1, col2, (sinf(2.0f * phi) * sinf(theta) + 1.0f) / 2.0f, p.col);
			points_.push_back(p);
		}
	}

	points_.push_back(bottomPoint);

	for (int i = 1; i <= sliceCount; ++i)
	{
		indices_.push_back(0);
		indices_.push_back(i + 1);
		indices_.push_back(i);
	}

	int baseIndex = 1;
	const int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount - 2; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			indices_.push_back(baseIndex + i * ringVertexCount + j);
			indices_.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices_.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	const int southPoleIndex = points_.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < sliceCount; ++i)
	{
		indices_.push_back(southPoleIndex);
		indices_.push_back(baseIndex + i);
		indices_.push_back(baseIndex + i + 1);
	}
}

void SphereComponent::Initialize()
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
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(points_) * 2;

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
	constBufDesc.ByteWidth = sizeof(SimpleMath::Matrix);

	game->device_->CreateBuffer(&constBufDesc, nullptr, &const_buffer_);

}

void SphereComponent::Update()
{
	const Matrix world = Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
	Matrix worldViewProj = world * game->Camera->GetMatrix();
	worldViewProj = worldViewProj.Transpose();
	game->context_->UpdateSubresource(const_buffer_, 0, nullptr, &worldViewProj, 0, 0);
}

void SphereComponent::Draw()
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

	game->context_->DrawIndexed(indices_.size(), 0, 0);
}

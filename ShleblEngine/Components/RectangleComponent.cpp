#include "RectangleComponent.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;


RectangleComponent::RectangleComponent(Game* g, const wchar_t* diffuseTextureName) : BaseComponent(g)
{
	isSpinningFloor = 1.0f;
	textureFileName_ = diffuseTextureName;

	points_.push_back({ Vector4(0.5f, 0.5f, 0.0f, 1.0f),	Vector4(64.0f, 64.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(-0.5f, -0.5f, 0.0f, 1.0f),Vector4(0.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector4(0.0f, 64.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	points_.push_back({ Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector4(64.0f, 0.0f, 0.0f, 0.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f) });

	indices_ = { 0,1,2, 1,0,3 };
}
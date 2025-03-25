#pragma once

#include "../Dependencies/SimpleMath.h"

class Game;

class GameComponent
{
protected:
	Game* game;
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 scale;
public:
	virtual void SetPosition(DirectX::SimpleMath::Vector3 p) { position = p; }
	virtual void SetRotation(DirectX::SimpleMath::Quaternion q) { rotation = q; }
	virtual void SetScale(DirectX::SimpleMath::Vector3 s) { scale = s; }
	GameComponent(Game* g);
	virtual ~GameComponent() = default;
	virtual DirectX::SimpleMath::Vector3 GetPosition() const { return position; }
	virtual DirectX::SimpleMath::Quaternion GetRotation() const { return rotation; }
	virtual DirectX::SimpleMath::Vector3 GetScale() const { return scale; }
	virtual void DestroyResources() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
};
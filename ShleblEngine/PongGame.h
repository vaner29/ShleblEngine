#pragma once
#include "Game.h"
#include <d2d1.h>
#include <dwrite.h>
#include "RectangleComponent.h"
#include "PongBall.h"

#define RIGHT_BOT

enum GameState
{
	PONG_STATE_NORMAL,
	PONG_STATE_COOLDOWN,
	PONG_STATE_GAMEOVER
};

class PongGame : public Game
{
protected:
	void SetBackgroundColor() override;
	unsigned s1;
	unsigned s2;
	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWFactory;
	IDWriteTextFormat* DWTextFormat;
	ID2D1RenderTarget* D2DRenderTarget;
	ID2D1SolidColorBrush* D2Dbrush;
	ID2D1StrokeStyle* D2DLineStrokeStyle;
	IDXGISurface* D2DBackBuff;
public:
	std::vector<DirectX::XMFLOAT4> hits_;
	int score_[2] = { 0, 0 };
	std::chrono::steady_clock::time_point ContinueTime;
	unsigned ballHits;
	GameState state;
	PongGame();
	RectangleComponent* racket1_;
	RectangleComponent* racket2_;
	std::vector<PongBall*> balls_;
	//void Draw() override;
	//void Initialize() override;
	void DestroyResources() override;
	//void IncScore(bool p);
	void Update() override;
	std::vector<DirectX::XMFLOAT4> GeneratePoints(float radius, float density);
	void MakeNewBall(DirectX::XMFLOAT2 position, DirectX::SimpleMath::Vector2 direction);
	std::vector<int> GenerateIndices(float density);

};

//#include "PongGame.h"
//#include <random>
//
//void PongGame::SetBackgroundColor()
//{
//	float color[] = { 0, 0, 0, 1.0f };
//	context_->ClearRenderTargetView(render_view_, color);
//}
//
//PongGame::PongGame() : Game(L"MyGame", 800, 800)
//{
//	std::vector<DirectX::XMFLOAT4> points = {
//		DirectX::XMFLOAT4(0.02f, 0.2f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(-0.02f, -0.2f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(-0.02f, 0.2f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//		DirectX::XMFLOAT4(0.02f, -0.2f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	};
//	std::vector<int> indices = { 0,1,2, 0,1,3 };
//	racket1_ = new RectangleComponent(this, points, indices);
//	racket1_->SetPosition(DirectX::XMFLOAT2(-0.95f, 0));
//	components_.push_back(racket1_);
//	racket2_ = new RectangleComponent(this, points, indices);
//	racket2_->SetPosition(DirectX::XMFLOAT2(0.95f, 0));
//	components_.push_back(racket2_);
//	//std::vector<DirectX::XMFLOAT4> points2 = {
//	//	DirectX::XMFLOAT4(0.02f, 0.02f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	//	DirectX::XMFLOAT4(-0.02f, -0.02f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	//	DirectX::XMFLOAT4(-0.02f, 0.02f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	//	DirectX::XMFLOAT4(0.02f, -0.02f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//	//};
//	DirectX::XMFLOAT4 center = DirectX::XMFLOAT4(1.0f, 1.0f, 0, 1.0f);
//	float density = 50;
//	float radius = 0.03f;
//	DirectX::XMFLOAT4 center2 = DirectX::XMFLOAT4(1.0f, 1.0f, 0, 1.0f);
//	PongBall* ball1 = new PongBall(this, this->GeneratePoints(radius, density), this->GenerateIndices(density));
//	balls_.push_back(ball1);
//	components_.push_back(ball1);
//}
//
//void PongGame::DestroyResources()
//{
//	//delete ball_;
//	Game::DestroyResources();
//}
//
//void PongGame::Update()
//{
//	//racket1_->SetX(racket1_->GetX() + 0.01f);
//	if (input_dev_->IsKeyDown(Keys::W) && racket1_->GetY() < 0.78f)
//		racket1_->SetY(racket1_->GetY() + delta_time_ * 1.3f);
//	if (input_dev_->IsKeyDown(Keys::S) && racket1_->GetY() > -0.78f) {
//		//this->MakeNewBall(DirectX::XMFLOAT2(0, 0), DirectX::SimpleMath::Vector2(0.0f, 0.0f));
//		racket1_->SetY(racket1_->GetY() - delta_time_ * 1.3f);
//	}
//
//
//	if (abs(racket2_->GetY() - balls_[0]->GetY()) > 0.1f)
//	{
//		if (racket2_->GetY() < balls_[0]->GetY() && racket2_->GetY() < 0.8f)
//			racket2_->SetY(racket2_->GetY() + delta_time_ * 1.1f);
//		if (racket2_->GetY() > balls_[0]->GetY() && racket2_->GetY() > -0.8f)
//			racket2_->SetY(racket2_->GetY() - delta_time_ * 1.1f);
//	}
//	for (int c = 0; c < std::size(components_); ++c)
//	{
//		components_[c]->Update();
//		//this->MakeNewBall(DirectX::XMFLOAT2(0, 0), DirectX::SimpleMath::Vector2(0.0f, 0.0f));
//	}
//	int size_before = components_.size();
//	std::default_random_engine generator(
//		std::chrono::system_clock::now().time_since_epoch().count());
//	std::uniform_real_distribution<float> distribution(-0.8, 0.8);
//	for (auto h : hits_) {
//		MakeNewBall(DirectX::XMFLOAT2(h.x, h.y), DirectX::SimpleMath::Vector2(h.z, distribution(generator)));
//		MakeNewBall(DirectX::XMFLOAT2(h.x, h.y), DirectX::SimpleMath::Vector2(h.z, distribution(generator)));
//		MakeNewBall(DirectX::XMFLOAT2(h.x, h.y), DirectX::SimpleMath::Vector2(h.z, distribution(generator)));
//	}
//	if (!hits_.empty())
//		for (int i = size_before; i < components_.size(); ++i)
//			components_[i]->Update();
//	hits_ = {};
//
//	for (auto d : ballsToDelete) {
//		balls_.erase(std::next(balls_.begin(), d));
//		components_.erase(std::next(components_.begin(), d+2));
//	}
//	ballsToDelete = {};
//	//Game::Update();
//}
//
//std::vector<DirectX::XMFLOAT4> PongGame::GeneratePoints(float radius, float density) {
//	std::vector<DirectX::XMFLOAT4> points;
//	points.push_back(DirectX::XMFLOAT4(0, 0, 0, 1.0f));
//	points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)); // center point
//	for (float i = 0; i <= density; i += 1.0f) {
//		float t = 3.14f * 2.0f * i / density;
//		points.push_back(DirectX::XMFLOAT4(radius * cos(t), radius * -sin(t), 0, 1.0f));
//		points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//	return points;
//}
//
//void PongGame::MakeNewBall(DirectX::XMFLOAT2 position, DirectX::SimpleMath::Vector2 direction)
//{
//	float density = 50;
//	float radius = 0.03f;
//	PongBall* ball2 = new PongBall(this, this->GeneratePoints(radius, density), this->GenerateIndices(density), position);
//	ball2->Initialize();
//	//ball2->SetPosition(position);
//	ball2->direction_ = DirectX::XMFLOAT2(direction.x, direction.y);
//	balls_.push_back(ball2);
//	components_.push_back(ball2);
//	return;
//}
//
//std::vector<int> PongGame::GenerateIndices(float density) {
//	std::vector<int> indices;
//	for (float i = 0; i <= density - 1; i += 1.0f) {
//		indices.push_back(0);
//		indices.push_back(i);
//		indices.push_back(i + 1);
//	}
//	indices.push_back(0);
//	indices.push_back(density - 1);
//	indices.push_back(1);
//	return indices;
//}
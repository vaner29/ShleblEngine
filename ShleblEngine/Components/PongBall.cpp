//#include "PongBall.h"
//#include "PongGame.h"
//
//void PongBall::GenRndDirection()
//{
//	DirectX::SimpleMath::Vector2::Lerp(DirectX::SimpleMath::Vector2(1.0f, 1.0f), DirectX::SimpleMath::Vector2(1.0f, -1.0f), (float)rand() / RAND_MAX, direction_);
//	//direction_ = SimpleMath::Vector2(1.0f, 0.0f);
//	if ((float)rand() / RAND_MAX < 0.5f)
//		direction_.x *= -1;
//	direction_.Normalize();
//}
//
//PongBall::PongBall(Game* g) : RectangleComponent(g)
//{
//	p_game_ = dynamic_cast<PongGame*>(g);
//	Reset();
//}
//
////PongBall::PongBall(Game* g, float client_radius, float client_density)
////	: RectangleComponent(g)
////{
////	radius_ = client_radius;
////	density_ = client_density;
////	points_ = GeneratePoints(client_radius, client_density);
////	indices_ = GenerateIndices(client_density);
////	p_game_ = dynamic_cast<PongGame*>(g);
////	Reset();
////}
////
////std::vector<DirectX::XMFLOAT4> PongBall::GeneratePoints(float radius, float density) {
////	std::vector<DirectX::XMFLOAT4> points;
////	points.push_back(DirectX::XMFLOAT4(0, 0, 0, 1.0f)); // center point
////	for (float i = 0; i <= density; i += 1.0f) {
////		float t = 3.14f * 2 * i / 100;
////		points.push_back(DirectX::XMFLOAT4(radius * cos(t), 0, radius * -sin(t), 1.0f));
////	}
////	return points;
////}
////
////std::vector<int> PongBall::GenerateIndices(float density) {
////	std::vector<int> indices;
////	for (float i = 0; i <= density - 1; i += 1.0f) {
////		indices.push_back(0);
////		indices.push_back(i);
////		indices.push_back(i + 1);
////	}
////	return indices;
////}
//
//PongBall::PongBall(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices): RectangleComponent(g, client_points, client_indices)
//{
//	p_game_ = dynamic_cast<PongGame*>(g);
//	Reset();
//}
//
//PongBall::PongBall(Game* g, std::vector<DirectX::XMFLOAT4> client_points, std::vector<int> client_indices, DirectX::XMFLOAT2 client_offset) : RectangleComponent(g, client_points, client_indices, client_offset)
//{
//	p_game_ = dynamic_cast<PongGame*>(g);
//	Reset();
//}
//
//void PongBall::Update()
//{
//	auto nextPos = GetPosition() + direction_ * speed_ * game->delta_time_;
//
//	if (nextPos.x < -0.93f) {
//		float racketY = p_game_->racket1_->GetY();
//		if (nextPos.y < racketY + 0.2f && nextPos.y > racketY - 0.2f) {
//			if (direction_.x < 0) { 
//				if (this->GetY() > racketY + 0.2f)
//					direction_.y = abs(direction_.y);
//				else if (this->GetY() < racketY - 0.2f)
//					direction_.y = -abs(direction_.y);
//
//				direction_.x = abs(direction_.x);
//				speed_ *= 1.1f;
//
//				p_game_->hits_.push_back(DirectX::XMFLOAT4(this->GetX(), this->GetY(), this->direction_.x, 0));
//			}
//		}
//	}
//
//	else if (nextPos.x > 0.93f) {
//		float racketY = p_game_->racket2_->GetY();
//		if (nextPos.y < racketY + 0.2f && nextPos.y > racketY - 0.2f) {
//			if (direction_.x > 0) {
//				if (this->GetY() > racketY + 0.2f)
//					direction_.y = abs(direction_.y);
//				else if (this->GetY() < racketY - 0.2f)
//					direction_.y = -abs(direction_.y);
//
//				direction_.x = -abs(direction_.x);
//				speed_ *= 1.1f;
//
//				p_game_->hits_.push_back(DirectX::XMFLOAT4(this->GetX(), this->GetY(), this->direction_.x, 0));
//			}
//		}
//	}
//		
//	if (nextPos.y > 1.0f) {
//		direction_.y = -abs(direction_.y);
//	}
//		
//	else if (nextPos.y < -1.0f) {
//		direction_.y = abs(direction_.y);
//	}
//	
//	else if (nextPos.x < -1.0f) {
//		if (p_game_->balls_.size() <= p_game_->ballsToDelete.size() + 1) {
//			p_game_->score_[1]++;
//			std::cout << "Too bad! \nScore - You " << p_game_->score_[0] << ":" << p_game_->score_[1] << " AI\n";
//			nextPos = DirectX::XMFLOAT2(0, 0);
//			Reset();
//		}
//		else {
//			p_game_->ballsToDelete.push_back(std::find(p_game_->balls_.begin(), p_game_->balls_.end(), this) - p_game_->balls_.begin());
//			//this->DestroyResources();
//			//return;
//		}
//	}	
//	else if (nextPos.x > 1.0f) {
//		if (p_game_->balls_.size() <= p_game_->ballsToDelete.size() + 1) {
//			p_game_->score_[0]++;
//			std::cout << "Nice! \nScore - You " << p_game_->score_[0] << ":" << p_game_->score_[1] << " AI\n";
//			nextPos = DirectX::XMFLOAT2(0, 0);
//			Reset();
//		}
//		else {
//			p_game_->ballsToDelete.push_back(std::find(p_game_->balls_.begin(), p_game_->balls_.end(), this) - p_game_->balls_.begin());
//			//this->DestroyResources();
//			//return;
//		}
//	}
//	SetPosition(nextPos);
//	RectangleComponent::Update();
//}
//
//void PongBall::Reset()
//{	
//	speed_ = 0.9f;
//	GenRndDirection();
//}
//
//void PongBall::SetPosition(DirectX::XMFLOAT2 pos)
//{
//	offset_.x = pos.x;
//	offset_.y = pos.y;
//}
//
//void PongBall::SetY(float y)
//{
//	offset_.y = y;
//}
//
//void PongBall::SetX(float x)
//{
//	offset_.x = x;
//}
//
//float PongBall::GetX() const
//{
//	return offset_.x;
//}
//
//float PongBall::GetY() const
//{
//	return offset_.y;
//}
//
//DirectX::XMFLOAT2 PongBall::GetPosition() const
//{
//	return DirectX::XMFLOAT2(offset_.x, offset_.y);
//}
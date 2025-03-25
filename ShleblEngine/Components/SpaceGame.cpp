#include "SpaceGame.h"
#include "GridComponent.h"
#include "SphereComponent.h"
#include "ObjectComponent.h"

using namespace DirectX;
using namespace SimpleMath;

void SpaceGame::SetBackgroundColor()
{
	float color[] = { 0, 0, 0, 1.0f };
	context_->ClearRenderTargetView(render_view_, color);
}

SpaceGame::SpaceGame() : Game(L"MyGame", 800, 800), controller (this)
{
	components_.push_back(new GridComponent(this, 1.0f, 1000));
	//components_.push_back(new SphereComponent(this, 1.0f, 32, 32, L"Textures/duck.dds"));
	components_.push_back(new ObjectComponent(this, "Models/duck.obj", L"Textures/duck.dds"));
	input_dev_->MouseMove.AddRaw(&controller, &CameraController::OnMouseMove);
}

void SpaceGame::Update()
{
	controller.Update();
	//if (controller.followShip){
	//	const float accelerationRate = 10.0f;  // How fast acceleration increases/decreases
	//	Vector3 forward = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
	//	forward.Normalize();
		/*if (input_dev_->IsKeyDown(Keys::Right))
		{
			Vector3 tmp = XMVector4Transform(Vector3::Left, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
			tmp.Normalize();
			ship->position += tmp * ship->speed * delta_time_;
		}
		if (input_dev_->IsKeyDown(Keys::Left))
		{
			Vector3 tmp = XMVector4Transform(Vector3::Right, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
			tmp.Normalize();
			ship->position += tmp * ship->speed * delta_time_;
		}*/
		//if (input_dev_->IsKeyDown(Keys::Up))
		//{
		//	//Vector3 tmp = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
		//	//tmp.Normalize();
		//	ship->velocity += forward * accelerationRate * delta_time_;
		//}
		//if (input_dev_->IsKeyDown(Keys::Down))
		//{
		//	//Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
		//	//tmp.Normalize();
		//	ship->velocity -= forward * accelerationRate * delta_time_;
		//}

		//if (input_dev_->IsKeyDown(Keys::Space))
		//{
		//	//Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
		//	//tmp.Normalize();
		//	ship->velocity = Vector3(0.0f, 0.0f, 0.0f);
		//}

		////std::cout << ship->velocity.x << " " << ship->velocity.y << " " << ship->velocity.z << std::endl;

		//// Assuming you have access to the camera's position, target, and up vectors
		//Vector3 cameraForward = this->Camera->Target - this->Camera->Position;
		//cameraForward.Normalize();
		//Vector3 cameraUp = this->Camera->Up;
		//cameraUp.Normalize();

		//const float focusOffset = 3.0f; // Adjust this value as needed
		//Vector3 focusPoint = ship->position + Vector3::Up * focusOffset; // Use world's up axis
		//// Alternatively, use the camera's up axis:
		//// Vector3 focusPoint = ship->position + up * focusOffset;

		//this->Camera->Target = focusPoint;

		//Vector3 cameraRight = cameraForward.Cross(cameraUp);
		//cameraRight.Normalize();

		//Vector3 shipDesiredForward = cameraRight;
		//Vector3 shipDesiredUp = cameraForward;    

		//const float tiltAngle = XMConvertToRadians(25.0f); 
		//Matrix tiltRotation = Matrix::CreateFromAxisAngle(cameraRight, tiltAngle);
		//shipDesiredUp = XMVector3Transform(shipDesiredUp, tiltRotation);
		//shipDesiredUp.Normalize();

		//Quaternion shipRotation = Quaternion::CreateFromRotationMatrix(
		//	Matrix::CreateWorld(Vector3::Zero, shipDesiredForward, shipDesiredUp)
		//);

		//ship->rotation = shipRotation;



	//}
	//ship->position += ship->velocity * delta_time_;

	//for (auto& pair : celestialBodies)
	//{
	//	ApplyGravity(pair.second, celestialBodies);
	//}

	Game::Update();


}

#include "SpaceGame.h"
#include "GridComponent.h"

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
	//// Sun
	CelestialBodyDesc sunDesc = {};
	sunDesc.SpinSpeed = 1.0f;
	sunDesc.Radius = 3.0f;
	sunDesc.PrimaryColor = Vector4(236.f / 180.f, 1.f, 134.f / 255.f, 1.f);
	sunDesc.SecondaryColor = Vector4(255.f / 255.f, 76.f / 255.f, 0.f, 1.f);
	//sunDesc.SecondaryColor = Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f);
	sunDesc.OrbitAnchor = nullptr;
	celestialBodies.insert({ "sun", new CelestialBody(this, sunDesc) });

	CelestialBodyDesc sun2Desc = {};
	sun2Desc.SpinSpeed = 1.0f;
	sun2Desc.Radius = 3.0f;
	sun2Desc.OrbitAnchor = nullptr;
	sun2Desc.PrimaryColor = Vector4(236.f / 180.f, 1.f, 134.f / 255.f, 1.f);
	sun2Desc.SecondaryColor = Vector4(255.f / 255.f, 76.f / 255.f, 0.f, 1.f);
	//sunDesc.SecondaryColor = Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f);
	celestialBodies.insert({ "betelgeuse", new CelestialBody(this, sun2Desc) });
	celestialBodies["betelgeuse"]->position = Vector3(35.0f, 0.0f, 0.0f);

	//// Mercury
	CelestialBodyDesc mercuryDesc = {};
	mercuryDesc.SpinSpeed = 1.0f;
	mercuryDesc.Radius = 1.0f;
	mercuryDesc.OrbitAnchor = celestialBodies["sun"];
	mercuryDesc.OrbitRadius = 4.0f;
	mercuryDesc.OrbitSpeed = 2.0f;
	mercuryDesc.PrimaryColor = Vector4(183.f / 255.f, 183.f / 255.f, 183.f / 255.f, 1.f);
	mercuryDesc.SecondaryColor = Vector4(153.f / 255.f, 65.f / 255.f, 99.f / 255.f, 1.f);
	//mercuryDesc.OrbitOrientationForward = XMVector4Transform(mercuryDesc.OrbitOrientationForward, Matrix::CreateFromAxisAngle(Vector3::Right, 7.0f * XM_PI / 180.0f));
	//mercuryDesc.OrbitOrientationUp = XMVector4Transform(mercuryDesc.OrbitOrientationUp, Matrix::CreateFromAxisAngle(Vector3::Right, 7.0f * XM_PI / 180.0f));
	celestialBodies.insert({ "mercury", new CelestialBody(this, mercuryDesc) });
	celestialBodies["mercury"]->position = Vector3(15.0f, 2.0f, 15.0f);

	//// Venus
	CelestialBodyDesc venusDesc = {};
	venusDesc.SpinSpeed = -0.8f;
	venusDesc.Radius = 1.0f;
	venusDesc.OrbitAnchor = celestialBodies["sun"];
	venusDesc.OrbitRadius = 7.0f;
	venusDesc.OrbitSpeed = 1.0f;
	//venusDesc.OrbitOrientationForward = XMVector4Transform(mercuryDesc.OrbitOrientationForward, Matrix::CreateFromAxisAngle(Vector3::Right, 3.0f * XM_PI / 180.0f));
	//venusDesc.OrbitOrientationUp = XMVector4Transform(mercuryDesc.OrbitOrientationUp, Matrix::CreateFromAxisAngle(Vector3::Right, 3.0f * XM_PI / 180.0f));
	venusDesc.PrimaryColor = Vector4(51.f / 255.f, 25.f / 255.f, 0.0f / 255.f, 1.f);
	//venusDesc.SecondaryColor = Vector4(51.f / 255.f, 25.f / 255.f, 0.0f / 255.f, 1.f);
	venusDesc.SecondaryColor = Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f);
	celestialBodies.insert({ "venus", new CelestialBody(this, venusDesc) });
	celestialBodies["venus"]->position = Vector3(-15.0f, -2.0f, 15.0f);

	//// Earth
	CelestialBodyDesc earthDesc = {};
	earthDesc.SpinSpeed = 100.f;
	earthDesc.Radius = 1.0f;
	earthDesc.OrbitAnchor = celestialBodies["sun"];
	earthDesc.OrbitRadius = 10.0f;
	earthDesc.OrbitSpeed = 0.0f;
	earthDesc.PrimaryColor = Vector4(0.0f, 0.0f, 0.4f, 1.0f);
	//earthDesc.SecondaryColor = Vector4(0.0f, 0.0f, 0.5f, 1.0f);
	earthDesc.SecondaryColor = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
	celestialBodies.insert({ "earth", new CelestialBody(this, earthDesc) });

	//// Moon
	CelestialBodyDesc moonDesc = {};
	moonDesc.SpinSpeed = 0.8f;
	moonDesc.Radius = 0.3f;
	moonDesc.OrbitAnchor = celestialBodies["earth"];
	moonDesc.OrbitRadius = 1.0f;
	moonDesc.OrbitSpeed = 5.0f;
	celestialBodies.insert({ "moon", new CelestialBody(this, moonDesc) });

	CelestialBodyDesc moon2Desc = {};
	moon2Desc.SpinSpeed = 0.8f;
	moon2Desc.Radius = 1.0f;
	moon2Desc.OrbitAnchor = celestialBodies["moon"];
	moon2Desc.OrbitRadius = 1.0f;
	moon2Desc.OrbitSpeed = 10.0f;
	celestialBodies.insert({ "moon2", new CelestialBody(this, moon2Desc) });

	//CelestialBodyDesc moon3Desc = {};
	//moon3Desc.SpinSpeed = 0.8f;
	//moon3Desc.Radius = 0.075f;
	//moon3Desc.OrbitAnchor = celestialBodies["moon2"];
	//moon3Desc.OrbitRadius = 0.5f;
	//moon3Desc.OrbitSpeed = 15.0f;
	//celestialBodies.insert({ "moon3", new CelestialBody(this, moon3Desc) });

	//CelestialBodyDesc moon4Desc = {};
	//moon4Desc.SpinSpeed = 0.8f;
	//moon4Desc.Radius = 0.0375f;
	//moon4Desc.OrbitAnchor = nullptr;
	//moon4Desc.OrbitRadius = 0.25f;
	//moon4Desc.OrbitSpeed = 20.0f;
	//celestialBodies.insert({ "moon4", new CelestialBody(this, moon4Desc) });
	//celestialBodies["moon4"]->position = Vector3(-2.0f, -2.0f, 2.0f);

	ship = new SpaceShip(this);
	celestialBodies.insert({ "ship", ship });

	for (const std::pair<const std::string, CelestialBody*>& n : celestialBodies)
		components_.push_back(n.second);

	input_dev_->MouseMove.AddRaw(&controller, &CameraController::OnMouseMove);

	//ID3DBlob* errorVertexCode = nullptr;
	//auto res = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
	//	nullptr /*macros*/,
	//	nullptr /*include*/,
	//	"VSMain",
	//	"vs_5_0",
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	//	0,
	//	&vertex_shader_byte_code_,
	//	&errorVertexCode);

	//if (FAILED(res)) {
	//	// If the shader failed to compile it should have written something to the error message.
	//	if (errorVertexCode) {
	//		const char* compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

	//		std::cout << compileErrors << std::endl;
	//	}
	//	// If there was  nothing in the error message then it simply could not find the shader file itself.
	//	else
	//	{
	//		MessageBox(display_->hwnd_, L"Base3dShader.hlsl", L"Missing Shader File", MB_OK);
	//	}

	//	return;
	//}

	//ID3DBlob* errorPixelCode;
	//res = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
	//	nullptr /*macros*/,
	//	nullptr /*include*/,
	//	"PSMain",
	//	"ps_5_0",
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	//	0,
	//	&pixel_shader_byte_code_,
	//	&errorPixelCode);

	//device_->CreateVertexShader(
	//	vertex_shader_byte_code_->GetBufferPointer(),
	//	vertex_shader_byte_code_->GetBufferSize(),
	//	nullptr, &vertex_shader_);

	//device_->CreatePixelShader(
	//	pixel_shader_byte_code_->GetBufferPointer(),
	//	pixel_shader_byte_code_->GetBufferSize(),
	//	nullptr, &pixel_shader_);
}

void SpaceGame::Update()
{
	controller.Update();
	if (controller.followShip){
		const float accelerationRate = 10.0f;  // How fast acceleration increases/decreases
		Vector3 forward = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
		forward.Normalize();
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
		if (input_dev_->IsKeyDown(Keys::Up))
		{
			//Vector3 tmp = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
			//tmp.Normalize();
			ship->velocity += forward * accelerationRate * delta_time_;
		}
		if (input_dev_->IsKeyDown(Keys::Down))
		{
			//Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
			//tmp.Normalize();
			ship->velocity -= forward * accelerationRate * delta_time_;
		}

		if (input_dev_->IsKeyDown(Keys::Space))
		{
			//Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(controller.yaw, controller.pitch, 0.0f));
			//tmp.Normalize();
			ship->velocity = Vector3(0.0f, 0.0f, 0.0f);
		}

		//std::cout << ship->velocity.x << " " << ship->velocity.y << " " << ship->velocity.z << std::endl;

		// Assuming you have access to the camera's position, target, and up vectors
		Vector3 cameraForward = this->Camera->Target - this->Camera->Position;
		cameraForward.Normalize();
		Vector3 cameraUp = this->Camera->Up;
		cameraUp.Normalize();

		const float focusOffset = 3.0f; // Adjust this value as needed
		Vector3 focusPoint = ship->position + Vector3::Up * focusOffset; // Use world's up axis
		// Alternatively, use the camera's up axis:
		// Vector3 focusPoint = ship->position + up * focusOffset;

		this->Camera->Target = focusPoint;

		Vector3 cameraRight = cameraForward.Cross(cameraUp);
		cameraRight.Normalize();

		Vector3 shipDesiredForward = cameraRight;
		Vector3 shipDesiredUp = cameraForward;    

		const float tiltAngle = XMConvertToRadians(25.0f); 
		Matrix tiltRotation = Matrix::CreateFromAxisAngle(cameraRight, tiltAngle);
		shipDesiredUp = XMVector3Transform(shipDesiredUp, tiltRotation);
		shipDesiredUp.Normalize();

		Quaternion shipRotation = Quaternion::CreateFromRotationMatrix(
			Matrix::CreateWorld(Vector3::Zero, shipDesiredForward, shipDesiredUp)
		);

		ship->rotation = shipRotation;



	}
	ship->position += ship->velocity * delta_time_;

	for (auto& pair : celestialBodies)
	{
		ApplyGravity(pair.second, celestialBodies);
	}

	Game::Update();

}

void SpaceGame::ApplyGravity(CelestialBody* currentBody, const std::unordered_map<std::string, CelestialBody*>& allBodies)
{
	const float G = 1000.0f;        
	const float maxSpeed = 45.0f; 
	const float minDistance = 0.1f;

	Vector3 netForce = Vector3::Zero;

	for (const auto& pair : allBodies)
	{
		CelestialBody* other = pair.second;
		if (other == currentBody) continue;

		Vector3 direction = other->position - currentBody->position;
		float distance = direction.Length();

		float clampedDistance = max(distance, minDistance);
		direction.Normalize();


		float mass1 = currentBody->radius_;
		float mass2 = other->radius_;

		float forceMagnitude = G * (mass1 * mass2) / (clampedDistance * clampedDistance);

		netForce += direction * forceMagnitude;
	}

	float mass = currentBody->radius_;
	currentBody->absoluteVelocity += (netForce / mass) * this->delta_time_;

	float speed = currentBody->absoluteVelocity.Length();
	if (speed > maxSpeed)
	{
		currentBody->absoluteVelocity = currentBody->absoluteVelocity * (maxSpeed / speed);
	}
}

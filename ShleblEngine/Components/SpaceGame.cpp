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
	// Sun
	CelestialBodyDesc sunDesc = {};
	sunDesc.SpinSpeed = 1.0f;
	sunDesc.Radius = 3.0f;
	sunDesc.PrimaryColor = Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f);
	sunDesc.SecondaryColor = Vector4(153.f / 255.f, 76.f / 255.f, 0.f, 1.f);
	celestialBodies.insert({ "sun", new CelestialBody(this, sunDesc) });

	// Earth
	CelestialBodyDesc earthDesc = {};
	earthDesc.SpinSpeed = 0.8f;
	earthDesc.Radius = 1.0f;
	earthDesc.OrbitAnchor = celestialBodies["sun"];
	earthDesc.OrbitRadius = 10.0f;
	earthDesc.OrbitSpeed = 0.8f;
	earthDesc.PrimaryColor = Vector4(0.0f, 0.0f, 0.5f, 1.0f);
	earthDesc.SecondaryColor = Vector4(0.0f, 0.7f, 0.0f, 1.0f);
	celestialBodies.insert({ "earth", new CelestialBody(this, earthDesc) });

	// Moon
	CelestialBodyDesc moonDesc = {};
	moonDesc.SpinSpeed = 0.8f;
	moonDesc.Radius = 0.3f;
	moonDesc.OrbitAnchor = celestialBodies["earth"];
	moonDesc.OrbitRadius = 2.0f;
	moonDesc.OrbitSpeed = 5.0f;
	celestialBodies.insert({ "moon", new CelestialBody(this, moonDesc) });

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
	Game::Update();
}

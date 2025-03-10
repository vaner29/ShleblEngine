#include "SpaceGame.h"
#include "GridComponent.h"

void SpaceGame::SetBackgroundColor()
{
	float color[] = { 0, 0, 0, 1.0f };
	context_->ClearRenderTargetView(render_view_, color);
}

SpaceGame::SpaceGame() : Game(L"MyGame", 800, 800), controller (this)
{
	components_.push_back(new GridComponent(this, 1.0f, 1000));
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

#include "Game.h"


LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	Game* pThis;

	if (umessage == WM_NCCREATE)
	{
		pThis = static_cast<Game*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		auto* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (pThis)
		{
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				/*printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.Reserved,
					raw->data.keyboard.ExtraInformation,
					raw->data.keyboard.Message,
					raw->data.keyboard.VKey);*/

				pThis->input_dev_->OnKeyDown({
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.VKey,
					raw->data.keyboard.Message
					});
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				pThis->input_dev_->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
			}
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

void Game::CreateBackBuffer()
{
	auto res = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));	// __uuidof(ID3D11Texture2D)
	res = device_->CreateRenderTargetView(back_buffer_, nullptr, &render_view_);
}

Game::Game(LPCWSTR name, int screen_width, int screen_height) : name_(name), frame_count_(0), components_()
{
	instance_ = GetModuleHandle(nullptr);

	display_ = new DisplayWin32(name, instance_, screen_width, screen_height, this);
	input_dev_ = new InputDevice(this);

	Camera = new ::Camera();
	Camera->AspectRatio = static_cast<float>(screen_width) / static_cast<float>(screen_height);

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 3;
	swapDesc.BufferDesc.Width = display_->client_width_;
	swapDesc.BufferDesc.Height = display_->client_height_;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display_->hwnd_;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swap_chain_,
		&device_,
		nullptr,
		&context_);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	CreateBackBuffer();

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device_->CreateRasterizerState(&rastDesc, &rast_state_);

	context_->RSSetState(rast_state_);

	ID3DBlob* errorVertexCode = nullptr;
	auto resShader = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertex_shader_byte_code_,
		&errorVertexCode);

	if (FAILED(resShader)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(display_->hwnd_, L"Base3dShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

	//D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	resShader = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixel_shader_byte_code_,
		&errorPixelCode);

	device_->CreateVertexShader(
		vertex_shader_byte_code_->GetBufferPointer(),
		vertex_shader_byte_code_->GetBufferSize(),
		nullptr, &vertex_shader_);

	device_->CreatePixelShader(
		pixel_shader_byte_code_->GetBufferPointer(),
		pixel_shader_byte_code_->GetBufferSize(),
		nullptr, &pixel_shader_);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = display_->client_width_;
	depthStencilDesc.Height = display_->client_height_;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT res2 = device_->CreateTexture2D(&depthStencilDesc, nullptr, &depth_stencil_buffer_);

	res2 = device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, &depth_stencil_view_);
}

Game::~Game()
{
	for (auto c : components_)
	{
		delete c;
	}
	delete display_;
	delete input_dev_;
}

void Game::Exit()
{
	DestroyResources();
}

void Game::MessageHandler()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		isExitRequested = true;
	}
}


void Game::Run()
{
	Initialize();
	isExitRequested = false;
	prev_time_ = std::chrono::steady_clock::now();
	MSG msg = {};
	while (!isExitRequested)
	{
		MessageHandler();

		auto	curTime = std::chrono::steady_clock::now();
		delta_time_ = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prev_time_).count() / 1000000.0f;
		prev_time_ = curTime;

		total_time_ += delta_time_;
		frame_count_++;

		if (total_time_ > 1.0f) {
			float fps = frame_count_ / total_time_;

			total_time_ -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(display_->hwnd_, text);

			frame_count_ = 0;
		}

		Update();

		PrepareFrame();

		Draw();

		context_->OMSetRenderTargets(0, nullptr, nullptr);

		swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}
	Exit();
}

void Game::PrepareFrame()
{

	context_->ClearState();

	context_->RSSetState(rast_state_);

	context_->OMSetRenderTargets(1, &render_view_, depth_stencil_view_);

	context_->VSSetShader(vertex_shader_, nullptr, 0);
	context_->PSSetShader(pixel_shader_, nullptr, 0);

	SetBackgroundColor();
	context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Game::SetBackgroundColor()
{
	float redValue = (std::sin(2 * 3.14f * (total_time_ + 0.0f)) + 1) / 2;
	float blueValue = (std::sin(2 * 3.14f * (total_time_ + 1.0f / 3.0f)) + 1) / 2;
	float greenValue = (std::sin(2 * 3.14f * (total_time_ + 2.0f / 3.0f)) + 1) / 2;
	float color[] = { redValue, blueValue, greenValue, 1.0f };
	context_->ClearRenderTargetView(render_view_, color);
}

void Game::DestroyResources()
{
	context_->Release();
	back_buffer_->Release();
	render_view_->Release();
	swap_chain_->Release();
	depth_stencil_view_->Release();
	depth_stencil_buffer_->Release();
	for (auto c : components_)
	{
		c->DestroyResources();
	}
}

void Game::Draw()
{

	for (auto c : components_)
	{
		c->Draw();
	}
}

void Game::Initialize()
{
	for (auto c : components_)
	{
		c->Initialize();
	}
}

void Game::Update()
{
	Camera->UpdateMatrix();
	for (auto c : components_)
	{
		c->Update();
	}
}



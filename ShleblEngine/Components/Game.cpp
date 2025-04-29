#include "Game.h"
#include "../Tools/DataProcesser.h"
#include "LightVolume.h"

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
				//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				//	raw->data.keyboard.MakeCode,
				//	raw->data.keyboard.Flags,
				//	raw->data.keyboard.Reserved,
				//	raw->data.keyboard.ExtraInformation,
				//	raw->data.keyboard.Message,
				//	raw->data.keyboard.VKey);

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

void Game::CreateDepthStencilBuffer()
{
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

	HRESULT res = device_->CreateTexture2D(&depthStencilDesc, nullptr, &depth_stencil_buffer_);

	res = device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, &depth_stencil_view_);
}

void Game::CreateCsmDepthTextureArray()
{
	D3D11_TEXTURE2D_DESC depthDescription = {};
	depthDescription.Width = 2048.0f;
	depthDescription.Height = 2048.0f;
	depthDescription.ArraySize = 5;
	depthDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthDescription.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDescription.MipLevels = 1;
	depthDescription.SampleDesc.Count = 1;
	depthDescription.SampleDesc.Quality = 0;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.MiscFlags = 0;

	auto res = device_->CreateTexture2D(&depthDescription, nullptr, &shadowTexArr_);

	if (FAILED(res))
	{
		OutputDebugString(TEXT("Fatal error: Failed to create CSM depth texture array!\n"));
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dViewDesc = {};
	dViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dViewDesc.Texture2DArray = {};
	dViewDesc.Texture2DArray.MipSlice = 0;
	dViewDesc.Texture2DArray.FirstArraySlice = 0;
	dViewDesc.Texture2DArray.ArraySize = 5;

	res = device_->CreateDepthStencilView(shadowTexArr_, &dViewDesc, &depthShadowDsv_);

	if (FAILED(res))
	{
		OutputDebugString(TEXT("Fatal error: Failed to create CSM depth stencil view!\n"));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray = {};
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 5;

	res = device_->CreateShaderResourceView(shadowTexArr_, &srvDesc, &depthShadowSrv_);

	if (FAILED(res))
	{
		OutputDebugString(TEXT("Fatal error: Failed to create CSM depth SRV!\n"));
	}
}

void Game::CreateBackBuffer()
{
	auto res = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));	// __uuidof(ID3D11Texture2D)
	res = device_->CreateRenderTargetView(back_buffer_, nullptr, &render_view_);
}

Game::Game(LPCWSTR name, int screen_width, int screen_height) : name_(name), frame_count_(0), dLight_(this), gBuffer_(this)
{
	instance_ = GetModuleHandle(nullptr);

	display_ = new DisplayWin32(name, instance_, screen_width, screen_height, this);
	input_dev_ = new InputDevice(this);

	Camera = new::Camera();
	Camera->AspectRatio = static_cast<float>(screen_width) / static_cast<float>(screen_height);

	lightVolumeComponent_ = new::LightVolume(this);
}

Game::~Game()
{
	for (auto c : components_)
	{
		delete c;
	}
	lightVolumeComponent_->~LightVolume();
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

void Game::InitTimer()
{
	prev_time_ = std::chrono::steady_clock::now();
}

void Game::UpdateTimer()
{
	auto	curTime = std::chrono::steady_clock::now();
	delta_time_ = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prev_time_).count() / 1000000.0f;
	prev_time_ = curTime;

	total_time_ += delta_time_;
	totalest_time_ += delta_time_;
	frame_count_++;

	if (total_time_ > 1.0f)
	{
		float fps = static_cast<float>(frame_count_) / total_time_;

		total_time_ -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(display_->hwnd_, text);

		frame_count_ = 0;
	}
}


void Game::Run()
{
	PrepareResources();
	Initialize();
	InitTimer();
	isExitRequested = false;
	while (!isExitRequested)
	{
		MessageHandler();

		UpdateTimer();

		Update();

		//CBDataPerScene sceneData = {};
		//sceneData.ambientStrength = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.2f); // 20% ambient strength
		//sceneData.viewPos = DirectX::SimpleMath::Vector4(this->Camera->Position.x, this->Camera->Position.y, this->Camera->Position.z, 1.0f);
		//sceneData.gTime = this->totalest_time_;
		//this->context_->UpdateSubresource(sceneConstantBuffer, 0, nullptr, &sceneData, 0, 0);


		PrepareFrame();

		Draw();

		EndFrame();
	}

	Exit();
}

void Game::PrepareFrame()
{

	context_->ClearState();

	context_->OMSetRenderTargets(0, nullptr, depthShadowDsv_);

	context_->ClearDepthStencilView(depthShadowDsv_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	for (const auto c : components_)
	{
		c->PrepareFrame();
	}
}

void Game::SetBackgroundColor()
{
	float redValue = (std::sin(2 * 3.14f * (total_time_ + 0.0f)) + 1) / 2;
	float blueValue = (std::sin(2 * 3.14f * (total_time_ + 1.0f / 3.0f)) + 1) / 2;
	float greenValue = (std::sin(2 * 3.14f * (total_time_ + 2.0f / 3.0f)) + 1) / 2;
	float color[] = { 0.0f,  0.0f,  0.0f, 1.0f };
	context_->ClearRenderTargetView(render_view_, color);
}

void Game::DestroyResources()
{
	//context_->Release();
	//back_buffer_->Release();
	//render_view_->Release();
	//swap_chain_->Release();
	//depth_stencil_view_->Release();
	//depth_stencil_buffer_->Release();
	for (auto c : components_)
	{
		c->DestroyResources();
	}
	lightVolumeComponent_->DestroyResources();
}

void Game::Draw()
{
	context_->ClearState();
	context_->OMSetDepthStencilState(defaultDepthState_, 0);

	const auto rtvs = new ID3D11RenderTargetView * [3];
	rtvs[0] = gBuffer_.albedoRtv_.Get();
	rtvs[1] = gBuffer_.positionRtv_.Get();
	rtvs[2] = gBuffer_.normalRtv_.Get();
	context_->OMSetRenderTargets(3, rtvs, depth_stencil_view_);

	constexpr float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context_->ClearRenderTargetView(gBuffer_.albedoRtv_.Get(), color);
	context_->ClearRenderTargetView(gBuffer_.positionRtv_.Get(), color);
	context_->ClearRenderTargetView(gBuffer_.normalRtv_.Get(), color);
	context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto c : components_)
	{
		c->Draw();
	}

	context_->ClearState();

	context_->RSSetState(rast_state_);
	context_->OMSetDepthStencilState(quadDepthState_, 0);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(display_->client_width_);
	viewport.Height = static_cast<float>(display_->client_height_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context_->RSSetViewports(1, &viewport);

	context_->OMSetRenderTargets(1, &render_view_, nullptr);

	context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context_->VSSetShader(DataProcesser::GetVertexShader("lightpass"), nullptr, 0);
	context_->PSSetShader(DataProcesser::GetPixelShader("lightpass"), nullptr, 0);
	context_->PSSetShaderResources(0, 1, gBuffer_.albedoSrv_.GetAddressOf());
	context_->PSSetShaderResources(1, 1, gBuffer_.positionSrv_.GetAddressOf());
	context_->PSSetShaderResources(2, 1, gBuffer_.normalSrv_.GetAddressOf());
	context_->PSSetShaderResources(3, 1, &depthShadowSrv_);
	context_->PSSetConstantBuffers(0, 1, &sceneConstantBuffer);
	context_->PSSetConstantBuffers(1, 1, &cascadeCBuffer_);
	context_->PSSetSamplers(0, 1, &depth_sampler_state_);

	context_->Draw(4, 0);
}

void Game::EndFrame()
{
	context_->OMSetRenderTargets(0, nullptr, nullptr);

	swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
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
	CBDataPerScene sceneData_ = {};
	auto tmp = DirectX::SimpleMath::Vector4(20.0f, 50.0f, 20.0f, 0.0f);
	tmp.Normalize();
	dLight_.SetDirection(tmp);
	sceneData_.LightPos = DirectX::SimpleMath::Vector4::Transform(dLight_.GetDirection(), Camera->GetView());
	sceneData_.LightPos.Normalize();
	sceneData_.LightColor = dLight_.GetColor();
	sceneData_.Ambient = 0.4f;
	sceneData_.Specular = 0.5f;
	sceneData_.Falloff = 32.f;
	sceneData_.Type = 0.f;
	sceneData_.T = DirectX::SimpleMath::Matrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	sceneData_.ViewMatrix = Camera->GetView();
	//sceneData_.gTime = this->totalest_time_;

	CbDataCascade cascadeData = {};
	auto tmp1 = GetDLight()->GetLightSpaceMatrices();
	for (int i = 0; i < 5; ++i)
	{
		cascadeData.ViewProj[i] = tmp1[i];
	}
	cascadeData.Distance = GetDLight()->GetShadowCascadeDistances();

	context_->UpdateSubresource(sceneConstantBuffer, 0, nullptr, &sceneData_, 0, 0);
	context_->UpdateSubresource(cascadeCBuffer_, 0, nullptr, &cascadeData, 0, 0);
	Camera->UpdateMatrix();
	for (const auto c : components_)
	{
		c->Update();
	}
}

void Game::UpdateInternal()
{
}

DirectionalLight* Game::GetDLight()
{
	return &dLight_;
}

ID3D11Buffer* const* Game::GetCascadeCb() const
{
	return &cascadeCBuffer_;
}

void Game::PrepareResources()
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
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
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
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

	CreateDepthStencilBuffer();

	CreateCsmDepthTextureArray();

	DataProcesser::Initialize(this);

	gBuffer_.Initialize();

	D3D11_BUFFER_DESC constBufPerSceneDesc = {};
	constBufPerSceneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufPerSceneDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufPerSceneDesc.CPUAccessFlags = 0;
	constBufPerSceneDesc.MiscFlags = 0;
	constBufPerSceneDesc.StructureByteStride = 0;
	constBufPerSceneDesc.ByteWidth = sizeof(CBDataPerScene);

	device_->CreateBuffer(&constBufPerSceneDesc, nullptr, &sceneConstantBuffer);

	D3D11_BUFFER_DESC constBufCascadeDesc;
	constBufCascadeDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufCascadeDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufCascadeDesc.CPUAccessFlags = 0;
	constBufCascadeDesc.MiscFlags = 0;
	constBufCascadeDesc.StructureByteStride = 0;
	constBufCascadeDesc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix) * 5 + sizeof(DirectX::SimpleMath::Vector4);

	device_->CreateBuffer(&constBufCascadeDesc, nullptr, &cascadeCBuffer_);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	res = device_->CreateRasterizerState(&rastDesc, &rast_state_);
	//context_->RSSetState(rast_state_);
	

	D3D11_DEPTH_STENCIL_DESC defaultDepthDesc = {};

	defaultDepthDesc.DepthEnable = true;
	defaultDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	defaultDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	res = device_->CreateDepthStencilState(&defaultDepthDesc, &defaultDepthState_);

	D3D11_DEPTH_STENCIL_DESC quadDepthDesc = {};

	defaultDepthDesc.DepthEnable = true;
	defaultDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	defaultDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	res = device_->CreateDepthStencilState(&quadDepthDesc, &quadDepthState_);

	D3D11_SAMPLER_DESC depthSamplerStateDesc = {};
	depthSamplerStateDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	depthSamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	depthSamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	depthSamplerStateDesc.BorderColor[0] = 1.0f;
	depthSamplerStateDesc.BorderColor[1] = 1.0f;
	depthSamplerStateDesc.BorderColor[2] = 1.0f;
	depthSamplerStateDesc.BorderColor[3] = 1.0f;

	res = device_->CreateSamplerState(&depthSamplerStateDesc, &depth_sampler_state_);

	//D3D11_BLEND_DESC blendDesc = {};

	//blendDesc.RenderTarget[0].BlendEnable = true;
	//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; //D3D11_BLEND_SRC_COLOR;
	//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; //D3D11_BLEND_BLEND_FACTOR;
	//blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;;
	//blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	//blendDesc.AlphaToCoverageEnable = false;

	//res = device_->CreateBlendState(&blendDesc, &blendState_);

	//lightVolumeComponent_->Initialize();
}


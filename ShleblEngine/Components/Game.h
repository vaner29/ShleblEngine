#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <cmath>


#include "GameComponent.h"
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "Camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game
{
private:
	bool isExitRequested;
	void CreateBackBuffer();

protected:
	virtual void SetBackgroundColor();
	virtual void DestroyResources();
	virtual void Draw();
	virtual void Initialize();
	virtual void Update();
	virtual void PrepareFrame();
public:
	ID3D11Texture2D* depth_stencil_buffer_;
	ID3D11DepthStencilView* depth_stencil_view_;
	Camera* Camera;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	ID3D11VertexShader* vertex_shader_;
	ID3DBlob* vertex_shader_byte_code_;
	ID3D11PixelShader* pixel_shader_;
	ID3DBlob* pixel_shader_byte_code_;
	ID3D11Texture2D* back_buffer_;
	ID3D11DeviceContext* context_;
	//int debug_annotation_;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	HINSTANCE instance_;
	LPCWSTR name_;
	std::chrono::steady_clock::time_point prev_time_;
	ID3D11Texture2D* render_srv_;
	ID3D11RenderTargetView* render_view_;
	ID3D11RasterizerState* rast_state_;
	//int screen_resized_;
	//float start_time_;
	IDXGISwapChain* swap_chain_;
	float total_time_;
	float delta_time_;
	std::vector<GameComponent*> components_;
	DisplayWin32* display_;
	InputDevice* input_dev_;
	unsigned int frame_count_;
	Game(LPCWSTR name, int screen_width, int screen_height);
	virtual ~Game();
	void Exit();
	void MessageHandler();
	void Run();



	// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
	// Debug program: F5 or Debug > Start Debugging menu

	// Tips for Getting Started: 
	//   1. Use the Solution Explorer window to add/manage files
	//   2. Use the Team Explorer window to connect to source control
	//   3. Use the Output window to see build output and other messages
	//   4. Use the Error List window to view errors
	//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
	//   6. In the future, to open this project again,
};


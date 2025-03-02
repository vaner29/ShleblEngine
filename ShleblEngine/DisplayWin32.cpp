#include "DisplayWin32.h"
#include "Game.h"

//DisplayWin32::DisplayWin32(LPCWSTR application_name, HINSTANCE h_inst, int screen_width, int screen_height, Game* g)
//{
//	h_instance_ = h_inst;
//	game = g;
//
//	wc_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wc_.lpfnWndProc = WndProc;
//	wc_.cbClsExtra = 0;
//	wc_.cbWndExtra = 0;
//	wc_.hInstance = h_instance_;
//	wc_.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
//	wc_.hIconSm = wc_.hIcon;
//	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wc_.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
//	wc_.lpszMenuName = nullptr;
//	wc_.lpszClassName = application_name;
//	wc_.cbSize = sizeof(WNDCLASSEX);
//
//	// Register the window class.
//	RegisterClassEx(&wc_);
//
//	client_width_ = screen_width;
//	client_height_ = screen_height;
//
//	RECT windowRect = { 0, 0, static_cast<LONG>(client_width_), static_cast<LONG>(client_height_) };
//	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
//
//	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
//
//	auto posX = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
//	auto posY = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
//
//	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, application_name, application_name,
//		dwStyle,
//		posX, posY,
//		windowRect.right - windowRect.left,
//		windowRect.bottom - windowRect.top,
//		nullptr, nullptr, h_instance_, nullptr);
//
//	ShowWindow(hwnd_, SW_SHOW);
//	SetForegroundWindow(hwnd_);
//	SetFocus(hwnd_);
//
//	ShowCursor(true);
//}
//
//#include "DisplayWin32.h"
//#include "Game.h"
//
//using namespace DirectX;

DisplayWin32::DisplayWin32(LPCWSTR applicationName, HINSTANCE hInst, int screenWidth, int screenHeight, Game* g)
{
	h_instance_ = hInst;
	game = g;

	wc_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc_.lpfnWndProc = Game::WndProc;
	wc_.cbClsExtra = 0;
	wc_.cbWndExtra = 0;
	wc_.hInstance = h_instance_;
	wc_.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc_.hIconSm = wc_.hIcon;
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc_.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc_.lpszMenuName = nullptr;
	wc_.lpszClassName = applicationName;
	wc_.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc_);

	client_width_ = screenWidth;
	client_height_ = screenHeight;

	RECT windowRect = { 0, 0, static_cast<LONG>(client_width_), static_cast<LONG>(client_height_) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, h_instance_, game); //

	ShowWindow(hwnd_, SW_SHOW);
	SetForegroundWindow(hwnd_);
	SetFocus(hwnd_);

	ShowCursor(true);

	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
	{
		// Registration failed. Call GetLastError for the cause of the error
	}
}

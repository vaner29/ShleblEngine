#pragma once
#include <windows.h>
#include <WinUser.h>
#include <iostream>

class Game;

class DisplayWin32
{
private:
	Game* game;
public:
	int client_height_;
	int client_width_;
	HINSTANCE h_instance_;
	HWND hwnd_;
	WNDCLASSEX wc_;
	DisplayWin32(LPCWSTR application_name, HINSTANCE h_inst, int screen_width, int screen_height, Game* g);
};
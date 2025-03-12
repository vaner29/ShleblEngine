// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Dependencies/pch.h"
#include "Components/Game.h"
#include "Components/SpaceGame.h"
#include "Components/PongGame.h"
#include "Components/BaseComponent.h"
#include "Components/RectangleComponent.h"
#include "Components/SphereComponent.h"
#include <vector>

int main() {
	//PongGame mygame = PongGame();
	//Game mygame = Game(L"MyGame", 800, 800);
	SpaceGame mygame = SpaceGame();
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, 1.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};

	//std::vector<DirectX::XMFLOAT2> points2 = {
	//	DirectX::XMFLOAT2(0.5f, 0.5f),	//DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(-0.5f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	DirectX::XMFLOAT2(-0.5f, 0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(0.5f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(-1.0f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(0.5f, 1.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//};
	std::vector<int> indices = { 0,1,2, 0,1,3 };
	//mygame.components_.push_back(new BaseComponent(&mygame));
	//mygame.components_.push_back(new BaseComponent(&mygame, points, indices));
	//mygame.components_.push_back(new SphereComponent(&mygame, 0.5f, 32, 32, DirectX::SimpleMath::Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f), DirectX::SimpleMath::Vector4(236.f / 255.f, 76.f, 134.f / 255.f, 1.f)));

	mygame.Run();
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

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
#include "KatamariComponents/KatamariGame.h"

int main() {
	//PongGame mygame = PongGame();
	//Game mygame = Game(L"MyGame", 800, 800);
	//SpaceGame mygame = SpaceGame();
	KatamariGame mygame = KatamariGame();
	/*std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -0.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, 1.5f, 0, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};*/

	//std::vector<DirectX::XMFLOAT2> points2 = {
	//	DirectX::XMFLOAT2(0.5f, 0.5f),	//DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(-0.5f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//	DirectX::XMFLOAT2(-0.5f, 0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(0.5f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(-1.0f, -0.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//	DirectX::XMFLOAT2(0.5f, 1.5f),	//DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//};
	//std::vector<int> indices = { 0,1,2, 0,1,3 };
	//mygame.components_.push_back(new BaseComponent(&mygame));
	//mygame.components_.push_back(new SphereComponent(&mygame, 0.5f, 32, 32, DirectX::SimpleMath::Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f), DirectX::SimpleMath::Vector4(236.f / 255.f, 76.f, 134.f / 255.f, 1.f)));
	//mygame.components_.push_back(new BaseComponent(&mygame, points, indices));
	//CelestialBodyDesc sunDesc = {};
	//sunDesc.SpinSpeed = 1.0f;
	//sunDesc.Radius = 3.0f;
	//sunDesc.PrimaryColor = Vector4(236.f / 255.f, 1.f, 134.f / 255.f, 1.f);
	//sunDesc.SecondaryColor = Vector4(153.f / 255.f, 76.f / 255.f, 0.f, 1.f);
	//mygame.celestialBodies.insert({ "sun", new CelestialBody(&mygame, sunDesc) });

	//// Earth
	//CelestialBodyDesc earthDesc = {};
	//earthDesc.SpinSpeed = 0.8f;
	//earthDesc.Radius = 1.0f;
	//earthDesc.OrbitAnchor = mygame.celestialBodies["sun"];
	//earthDesc.OrbitRadius = 10.0f;
	//earthDesc.OrbitSpeed = 0.8f;
	//earthDesc.PrimaryColor = Vector4(0.0f, 0.0f, 0.5f, 1.0f);
	//earthDesc.SecondaryColor = Vector4(0.0f, 0.7f, 0.0f, 1.0f);
	//mygame.celestialBodies.insert({ "earth", new CelestialBody(&mygame, earthDesc) });

	//// Moon
	//CelestialBodyDesc moonDesc = {};
	//moonDesc.SpinSpeed = 0.8f;
	//moonDesc.Radius = 0.3f;
	//moonDesc.OrbitAnchor = mygame.celestialBodies["earth"];
	//moonDesc.OrbitRadius = 2.0f;
	//moonDesc.OrbitSpeed = 5.0f;
	//mygame.celestialBodies.insert({ "moon", new CelestialBody(&mygame, moonDesc) });

	//for (const std::pair<const std::string, CelestialBody*>& n : mygame.celestialBodies)
	//	mygame.components_.push_back(n.second);

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

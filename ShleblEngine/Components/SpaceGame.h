#pragma once
#include "unordered_map"
#include "Game.h"
#include "CameraController.h"
#include "CelestialBody.h"
#include "SpaceShip.h"


class SpaceGame : public Game
{
protected:
	void SetBackgroundColor() override;
public:
	//std::unordered_map<std::string, CelestialBody*> celestialBodies;
	//SpaceShip* ship;
	CameraController controller;
	SpaceGame();
	//void DestroyResources() override;
	void Update() override;
	//void ApplyGravity(CelestialBody* currentBody, const std::unordered_map<std::string, CelestialBody*>& allBodies);
	//void Initialize() override;
	//void PrepareFrame() override;
};

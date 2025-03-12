#pragma once
#include "unordered_map"
#include "Game.h"
#include "CameraController.h"
#include "CelestialBody.h"


class SpaceGame : public Game
{
protected:
	void SetBackgroundColor() override;
public:
	std::unordered_map<std::string, CelestialBody*> celestialBodies;
	CameraController controller;
	SpaceGame();
	//void DestroyResources() override;
	void Update() override;
	//void Initialize() override;
	//void PrepareFrame() override;
};

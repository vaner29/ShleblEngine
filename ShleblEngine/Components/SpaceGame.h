#pragma once
#include "Game.h"
#include "CameraController.h"


class SpaceGame : public Game
{
protected:
	void SetBackgroundColor() override;
public:
	CameraController controller;
	SpaceGame();
	//void DestroyResources() override;
	//void Update() override;
	//void Initialize() override;
	//void PrepareFrame() override;
};

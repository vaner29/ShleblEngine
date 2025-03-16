#pragma once
#include "CelestialBody.h"
class SpaceShip :
    public CelestialBody
{
public:
    float speed = 10.f;
    SpaceShip(Game* game);
    void Update() override;

};

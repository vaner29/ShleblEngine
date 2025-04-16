#pragma once
#include "SphereComponent.h"

class LightVolume : public SphereComponent
{
protected:
    CD3D11_RASTERIZER_DESC CreateRasterizerStateDesc() override;
public:
    ID3D11RasterizerState* rastState_;
    explicit LightVolume(Game* g);
    void SetSize(float size);
    void Draw() override;
};

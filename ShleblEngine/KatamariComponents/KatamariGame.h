#pragma once

#include "../Components/Game.h"
#include "../Components/CameraController.h"

class KatamariTrash;
class KatamariBall;

struct PointLight {
    DirectX::SimpleMath::Vector3 position;
    bool active = false;
    DirectX::SimpleMath::Vector3 color = { 1.0f, 1.0f, 1.0f };
    float intensity = 1.0f;
    DirectX::SimpleMath::Vector3 velocity;
    DirectX::SimpleMath::Matrix lightViewProj[4];
};

class KatamariGame : public Game
{
protected:
    CameraController cameraController;
    std::vector<SphereComponent*> lightSpheres;
    float shootDistance = 100.0f;
    DirectX::SimpleMath::Vector3 lightPoolPosition = { 0.0f, -1.0f, 0.0f };
    void ShootPointLight();
    void UpdateObjectLights(GameComponent* obj);
public:
    std::vector<PointLight> pointLights;
    KatamariBall* ball;
    std::vector<KatamariTrash*> furniture{};
    KatamariGame();
    void Update() override;
    //void PrepareFrame() override;
    void Initialize() override;
    void Draw() override;

    // Shadow map resources
    static const int NumCascades = 4;
    static const int ShadowMapSize = 1024; // Resolution per cascade
    ID3D11Texture2D* shadowMapTexture = nullptr;
    ID3D11DepthStencilView* shadowMapDSVs[NumCascades] = { nullptr }; // One DSV per cascade
    ID3D11ShaderResourceView* shadowMapSRV = nullptr;
    ID3D11DepthStencilState* shadowDepthState = nullptr;
    bool isDebug = false;
    //void RenderShadowMaps();
};
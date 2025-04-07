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
};

class KatamariGame : public Game
{
protected:
    CameraController cameraController;
    std::vector<SphereComponent*> lightSpheres;
    float shootDistance = 100.0f;
    DirectX::SimpleMath::Vector3 lightPoolPosition = { 0.0f, 1.0f, 0.0f };
    void ShootPointLight();
    void UpdateObjectLights(GameComponent* obj);
public:
    std::vector<PointLight> pointLights;
    KatamariBall* ball;
    std::vector<KatamariTrash*> furniture{};
    KatamariGame();
    void Update() override;
    void PrepareFrame() override;
    void Initialize() override;
};
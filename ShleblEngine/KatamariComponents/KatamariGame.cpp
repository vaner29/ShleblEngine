#pragma once﻿
#include "KatamariGame.h"
#include "../Components/GridComponent.h"
#include "KatamariTrash.h"
#include "../Components/RectangleComponent.h"
#include "KatamariBall.h"
#include "../Tools/DataProcesser.h"

using namespace DirectX;
using namespace SimpleMath;

KatamariGame::KatamariGame() : Game(L"Katamari Game", 800, 800), cameraController(this)
{
    srand(static_cast<unsigned>(time(nullptr)));

    ball = new KatamariBall(this);
    ball->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    components_.push_back(ball);

    RectangleComponent* quad = new RectangleComponent(this, L"Textures/square.dds");
    quad->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Left, XM_PI / 2.0f));
    quad->SetScale(Vector3::One * 1000.0f);
    components_.push_back(quad);

    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* cat = new KatamariTrash(this, "Models/maxwell.obj", L"Textures/dingus_nowhiskers.dds", 1.3f, Vector3(0.0f, 0.0f, 0.0f));
        cat->SetScale(Vector3(0.0009f, 0.0009f, 0.0009f));
        cat->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        cat->collision.Radius = 0.4f;
        cat->shininess = 1000.f;
        cat->specularColor = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
        cat->diffuseColor = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
        components_.push_back(cat);
        furniture.push_back(cat);
    }
    input_dev_->MouseMove.AddRaw(&cameraController, &CameraController::OnMouseMove);

    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* gabriel = new KatamariTrash(this, "Models/Gabriel.obj", L"Textures/gabriel.dds", 0.9f, Vector3(0.0f, 0.0f, 0.0f));
        gabriel->SetScale(Vector3(0.005f, 0.005f, 0.005f));
        gabriel->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        gabriel->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Right, XM_PI / 2.0f));
        gabriel->shininess = 1000.f;
        gabriel->specularColor = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
        gabriel->collision.Radius = 0.1f;
        components_.push_back(gabriel);
        furniture.push_back(gabriel);
    }

    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* melon = new KatamariTrash(this, "Models/melon.obj", L"Textures/melon.dds", 2.0f, Vector3(0.0f, 0.0f, 0.0f));
        melon->SetScale(Vector3(10.f,10.f, 10.f));
        melon->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        melon->shininess = 1000.f;
        melon->specularColor = DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f);
        melon->diffuseColor = DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f);
        melon->collision.Radius = 1.f;
        components_.push_back(melon);
        furniture.push_back(melon);
    }
    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* cucumber = new KatamariTrash(this, "Models/cucumber.obj", L"Textures/cucumber.dds", 0.9f, Vector3(0.0f, 0.0f, 0.0f));
        cucumber->SetScale(Vector3(7.f, 7.f, 7.f));
        cucumber->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        cucumber->collision.Radius = 0.8f;
        cucumber->shininess = 1000.f;
        cucumber->specularColor = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
        components_.push_back(cucumber);
        furniture.push_back(cucumber);
    }

    KatamariTrash* bread = new KatamariTrash(this, "Models/bread.obj", L"Textures/bread.dds", 3.0f, Vector3(0.0f, 1.0f, 0.0f));
    bread->SetScale(Vector3(50.0f, 50.0f, 50.0f));
    bread->SetPosition(Vector3(30.0f, 0.0f, 10.0f));
    bread->collision.Radius = 2.5f;
    components_.push_back(bread);
    furniture.push_back(bread);
    cameraController.targetBall = ball;

    KatamariTrash* car = new KatamariTrash(this, "Models/car.obj", L"Textures/car.dds", 7.0f, Vector3(0.0f, 1.0f, 0.0f));
    car->SetScale(Vector3(30.0f, 30.0f, 30.0f));
    car->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Right, XM_PI / 2.0f));
    car->SetPosition(Vector3(-30.0f, 0.0f, 10.0f));
    car->collision.Radius = 10.f;
    components_.push_back(car);
    furniture.push_back(car);
    cameraController.targetBall = ball;
}

void KatamariGame::Update()
{
    cameraController.Update();
    Vector3 dir = Vector3::Zero;
    if (input_dev_->IsKeyDown(Keys::W))
        //dir += CameraController->GetForward();
        dir += Camera->Target - Camera->Position;
    if (input_dev_->IsKeyDown(Keys::S))
        //dir -= CameraController->GetForward();
        dir -= Camera->Target - Camera->Position;
    if (input_dev_->IsKeyDown(Keys::A))
        //dir -= (CameraController->GetForward()).Cross(CameraController->GetUp());
        dir -= (Camera->Target - Camera->Position).Cross(Camera->Up);
    if (input_dev_->IsKeyDown(Keys::D))
        //dir += (CameraController->GetForward()).Cross(CameraController->GetUp());
        dir += (Camera->Target - Camera->Position).Cross(Camera->Up);
    if (dir.Length() > 0.0f)
        ball->SetDirection(dir);

    std::cout << ball->GetPosition().x << " " << ball->GetPosition().y << " " << ball->GetPosition().z << " " << std::endl;

    //if (input_dev_->IsKeyDown(Keys::E)) ShootPointLight();
    ball->Update();
    for (auto object : furniture)
        object->Update();
    // Update light positions
    for (size_t i = 0; i < pointLights.size(); i++) {
        auto& light = pointLights[i];
        if (light.active) {
            light.position += light.velocity * delta_time_;
            float distanceTraveled = (light.position - ball->GetPosition()).Length();
            if (distanceTraveled > shootDistance) {
                light.position = lightPoolPosition;
                light.active = false;
                light.velocity = Vector3::Zero;
            }
            lightSpheres[i]->SetPosition(light.position);
            lightSpheres[i]->diffuseColor = light.color;
        }
        else {
            lightSpheres[i]->SetPosition(lightPoolPosition);
        }
    }

    // Update lighting for each component
    for (auto* comp : components_) {
        UpdateObjectLights(comp);
    }
    Camera->UpdateMatrix();

   //Game::Update();
}


void KatamariGame::PrepareFrame()
{

    context_->ClearState();

    context_->RSSetState(rast_state_);

    context_->OMSetRenderTargets(1, &render_view_, depth_stencil_view_);

    context_->VSSetShader(DataProcesser::GetVertexShader("spinny"), nullptr, 0);
    context_->PSSetShader(DataProcesser::GetPixelShader("spinny"), nullptr, 0);

    context_->PSSetSamplers(0, 1, &sampler_state_);

    SetBackgroundColor();
    context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void KatamariGame::Initialize()
{
    pointLights.resize(100);
    lightSpheres.resize(100);
    for (int i = 0; i < 100; i++) {
        pointLights[i].position = lightPoolPosition;
        lightSpheres[i] = new SphereComponent(this, 0.2f, 16, 16, L"Textures/slime.dds");
        lightSpheres[i]->SetPosition(lightPoolPosition);
        components_.push_back(lightSpheres[i]);
    }
    input_dev_->KeyPressed.AddLambda([this](Keys key) {
        if (key == Keys::E) {
            ShootPointLight();
        }
        });

    // Initialize shadow map texture array
    D3D11_TEXTURE2D_DESC shadowTexDesc = {};
    shadowTexDesc.Width = ShadowMapSize;
    shadowTexDesc.Height = ShadowMapSize;
    shadowTexDesc.MipLevels = 1;
    shadowTexDesc.ArraySize = NumCascades * 4; // 4 lights, 4 cascades each
    shadowTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    shadowTexDesc.SampleDesc.Count = 1;
    shadowTexDesc.SampleDesc.Quality = 0;
    shadowTexDesc.Usage = D3D11_USAGE_DEFAULT;
    shadowTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    shadowTexDesc.CPUAccessFlags = 0;
    shadowTexDesc.MiscFlags = 0;
    device_->CreateTexture2D(&shadowTexDesc, nullptr, &shadowMapTexture);

    // Create DSVs for each cascade (for first 4 lights)
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    dsvDesc.Texture2DArray.MipSlice = 0;
    dsvDesc.Texture2DArray.ArraySize = 1; // One slice per DSV
    for (int i = 0; i < NumCascades; i++) {
        dsvDesc.Texture2DArray.FirstArraySlice = i; // Each cascade gets its own slice
        device_->CreateDepthStencilView(shadowMapTexture, &dsvDesc, &shadowMapDSVs[i]);
    }

    // Create SRV for the entire texture array
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MostDetailedMip = 0;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.FirstArraySlice = 0;
    srvDesc.Texture2DArray.ArraySize = NumCascades * 4;
    device_->CreateShaderResourceView(shadowMapTexture, &srvDesc, &shadowMapSRV);

    // Depth stencil state for shadow rendering
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    device_->CreateDepthStencilState(&dsDesc, &shadowDepthState);

    Game::Initialize();
}

void KatamariGame::ShootPointLight()
{
    for (size_t i = 0; i < pointLights.size(); i++) {
        if (!pointLights[i].active) {
            auto& light = pointLights[i];
            light.active = true;
            light.position = ball->GetPosition();
            Vector3 shootDir = (Camera->Target - Camera->Position);
            shootDir.Normalize();
            shootDir.y = 0;
            light.velocity = shootDir * 20.0f; // 20 units/sec
            light.color = Vector3(1.0f, 0.5f, 1.0f);
            lightSpheres[i]->SetPosition(light.position);
            lightSpheres[i]->diffuseColor = light.color;
            break;
        }
    }
}

void KatamariGame::UpdateObjectLights(GameComponent* obj)
{

    BaseComponent* baseComponent = dynamic_cast<BaseComponent*>(obj);

    baseComponent->rotation.Normalize();
    const Matrix world = Matrix::CreateScale(baseComponent->scale) * Matrix::CreateFromQuaternion(baseComponent->rotation) * Matrix::CreateTranslation(baseComponent->position);

    BaseComponent::CBDataPerObject objData = {};
    objData.worldViewProj = world * this->Camera->GetMatrix();
    objData.invTrWorld = objData.invTrWorld = (baseComponent->isSpinningFloor > 0.5f) ? Matrix::Identity : (Matrix::CreateScale(baseComponent->scale) * Matrix::CreateFromQuaternion(baseComponent->rotation)).Invert().Transpose();
    objData.world = world; // Set world matrix
    objData.isSpinningFloor = baseComponent->isSpinningFloor;
    objData.diffuseColor = baseComponent->diffuseColor;
    objData.specularColor = baseComponent->specularColor;
    objData.shininess = baseComponent->shininess;

    //baseComponent->Update(); // Fill basic data
    //this->context_->UpdateSubresource(baseComponent->const_buffers_[0], 0, nullptr, &objData, 0, 0); // Get current data

    // Find closest 4 lights
    std::vector<std::pair<float, size_t>> distances;
    Vector3 objPos = baseComponent->GetPosition();
    for (size_t i = 0; i < pointLights.size(); i++) {
        if (pointLights[i].active) {
            float dist = (pointLights[i].position - objPos).Length();
            if (dist < 100.0f) { // Only consider lights within radius
                distances.push_back({ dist, i });
            }
        }
    }
    std::sort(distances.begin(), distances.end()); // Sort by distance

    objData.numPointLights = std::min(int(distances.size()), 4);
    for (int i = 0; i < objData.numPointLights; i++) {
        const auto& light = pointLights[distances[i].second];
        objData.pointLights[i].position = Vector4(light.position.x, light.position.y, light.position.z, 1.0f);
        objData.pointLights[i].color = Vector4(light.color.x, light.color.y, light.color.z, light.intensity);
    }

    this->context_->UpdateSubresource(baseComponent->objConstantBuffer, 0, nullptr, &objData, 0, 0);
}

void KatamariGame::RenderShadowMaps()
{
    // Use shadow-specific shaders
    context_->VSSetShader(DataProcesser::GetVertexShader("shadow"), nullptr, 0);
    context_->PSSetShader(nullptr, nullptr, 0); // No pixel shader for depth-only

    context_->OMSetDepthStencilState(shadowDepthState, 0);
    context_->RSSetState(rast_state_);

    // Define cascade splits (in view space depth)
    float nearClip = 0.1f;
    float farClip = 50.0f;
    float cascadeSplits[NumCascades] = { farClip * 0.05f, farClip * 0.15f, farClip * 0.5f, farClip };

    // Find first active light (for simplicity, extend to multiple later)
    int lightIndex = -1;
    for (size_t i = 0; i < pointLights.size(); i++) {
        if (pointLights[i].active) {
            lightIndex = i;
            break;
        }
    }
    if (lightIndex == -1) return;

    PointLight& light = pointLights[lightIndex];
    Vector3 lightPos = light.position;
    Vector3 lightDir = -Vector3::Up; // Simplified; adjust based on light movement
    Vector3 up = Vector3::Forward;

    for (int i = 0; i < NumCascades; i++) {
        // Define frustum corners for this cascade
        float nearZ = (i == 0) ? nearClip : cascadeSplits[i - 1];
        float farZ = cascadeSplits[i];
        Matrix view = Camera->GetViewMatrix();
        Matrix proj = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV4, Camera->AspectRatio, nearZ, farZ);
        Matrix viewProj = view * proj;

        // Get frustum corners in world space
        Vector3 corners[8];
        float zNear = -nearZ, zFar = -farZ;
        float tanHalfFOV = tan(XM_PIDIV4 / 2.0f);
        float aspect = Camera->AspectRatio;
        float nearHeight = tanHalfFOV * nearZ;
        float nearWidth = nearHeight * aspect;
        float farHeight = tanHalfFOV * farZ;
        float farWidth = farHeight * aspect;

        corners[0] = Vector3(-nearWidth, nearHeight, zNear); // Near top-left
        corners[1] = Vector3(nearWidth, nearHeight, zNear);  // Near top-right
        corners[2] = Vector3(-nearWidth, -nearHeight, zNear); // Near bottom-left
        corners[3] = Vector3(nearWidth, -nearHeight, zNear);  // Near bottom-right
        corners[4] = Vector3(-farWidth, farHeight, zFar);     // Far top-left
        corners[5] = Vector3(farWidth, farHeight, zFar);      // Far top-right
        corners[6] = Vector3(-farWidth, -farHeight, zFar);    // Far bottom-left
        corners[7] = Vector3(farWidth, -farHeight, zFar);     // Far bottom-right

        Matrix invViewProj = viewProj.Invert();
        for (auto& corner : corners) {
            Vector4 transformed = Vector4::Transform(Vector4(corner.x, corner.y, corner.z, 1.0f), invViewProj);
            transformed /= transformed.w;
            corner = Vector3(transformed.x, transformed.y, transformed.z);
        }

        // Compute light view frustum
        Vector3 center = Vector3::Zero;
        for (const auto& corner : corners) center += corner;
        center /= 8.0f;
        Vector3 lightLookAt = center;
        Matrix lightView = Matrix::CreateLookAt(lightPos, lightLookAt, up);

        // Orthographic projection bounds
        float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX, minZ = FLT_MAX, maxZ = -FLT_MAX;
        for (const auto& corner : corners) {
            Vector3 lightSpace = Vector3::Transform(corner, lightView);
            minX = std::min(minX, lightSpace.x); maxX = max(maxX, lightSpace.x);
            minY = std::min(minY, lightSpace.y); maxY = max(maxY, lightSpace.y);
            minZ = std::min(minZ, lightSpace.z); maxZ = max(maxZ, lightSpace.z);
        }
        Matrix lightProj = Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, -maxZ, -minZ);
        light.lightViewProj[i] = lightView * lightProj;

        // Render to shadow map
        context_->OMSetRenderTargets(0, nullptr, shadowMapDSVs[i]);
        context_->ClearDepthStencilView(shadowMapDSVs[i], D3D11_CLEAR_DEPTH, 1.0f, 0);
        D3D11_VIEWPORT viewport = { 0, 0, (float)ShadowMapSize, (float)ShadowMapSize, 0.0f, 1.0f };
        context_->RSSetViewports(1, &viewport);

        for (auto* comp : components_) {
            comp->Update(); // Use light's view-proj in shadow pass
            comp->Draw();
        }
    }

    // Update CBDataPerScene with cascade splits
    CBDataPerScene sceneData = {};
    sceneData.ambientStrength = Vector4(0.0f, 0.0f, 0.0f, 0.2f);
    sceneData.viewPos = Vector4(Camera->Position.x, Camera->Position.y, Camera->Position.z, 1.0f);
    sceneData.gTime = totalest_time_;
    for (int i = 0; i < NumCascades; i++) {
        sceneData.cascadeSplits[i] = cascadeSplits[i];
    }
    context_->UpdateSubresource(sceneConstantBuffer, 0, nullptr, &sceneData, 0, 0);
}
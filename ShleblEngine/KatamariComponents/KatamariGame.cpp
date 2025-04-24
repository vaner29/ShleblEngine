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
    quad->shininess = 0.1f;
    quad->specularColor = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
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

    //ball->Update();
    //for (auto object : furniture)
        //object->Update();

    Game::Update();

    //Update light positions
    //for (size_t i = 0; i < pointLights.size(); i++) {
    //    auto& light = pointLights[i];
    //    if (light.active) {
    //        light.position += light.velocity * delta_time_;
    //        float distanceTraveled = (light.position - ball->GetPosition()).Length();
    //        if (distanceTraveled > shootDistance) {
    //            light.position = lightPoolPosition;
    //            light.active = false;
    //            light.velocity = Vector3::Zero;
    //        }
    //        lightSpheres[i]->SetPosition(light.position);
    //        lightSpheres[i]->diffuseColor = light.color;
    //    }
    //    else {
    //        lightSpheres[i]->SetPosition(lightPoolPosition);
    //    }
    //}

    // Update lighting for each component
    //for (auto* comp : components_) {
    //    UpdateObjectLights(comp);
    //}
    //Camera->UpdateMatrix();

   //Game::Update();
}


//void KatamariGame::PrepareFrame()
//{
//
//    context_->ClearState();
//
//    context_->RSSetState(rast_state_);
//
//    context_->OMSetRenderTargets(1, &render_view_, depth_stencil_view_);
//
//    SetBackgroundColor();
//    context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//    context_->VSSetShader(DataProcesser::GetVertexShader("csm"), nullptr, 0);
//    context_->GSSetShader(DataProcesser::GetGeometryShader("csm"), nullptr, 0);
//
//    context_->PSSetSamplers(0, 1, &sampler_state_);
//    context_->PSSetSamplers(1, 1, &depth_sampler_state_);
//
//
//
//    for (const auto c : components_)
//    {
//        c->PrepareFrame();
//    }
//}

//void KatamariGame::Initialize()
//{
//    //pointLights.resize(100);
//    //lightSpheres.resize(100);
//    //for (int i = 0; i < 100; i++) {
//    //    pointLights[i].position = lightPoolPosition;
//    //    lightSpheres[i] = new SphereComponent(this, 0.2f, 16, 16, L"Textures/slime.dds");
//    //    lightSpheres[i]->SetPosition(lightPoolPosition);
//    //    components_.push_back(lightSpheres[i]);
//    //}
//    input_dev_->KeyPressed.AddLambda([this](Keys key) {
//        if (key == Keys::E) {
//            ShootPointLight();
//        }
//        });
//
//    input_dev_->KeyPressed.AddLambda([this](Keys key) {
//        if (key == Keys::Space) {
//            isDebug = !isDebug;
//        }
//        });
//
//    //// Initialize shadow map texture array
//    //D3D11_TEXTURE2D_DESC shadowTexDesc = {};
//    //shadowTexDesc.Width = ShadowMapSize;
//    //shadowTexDesc.Height = ShadowMapSize;
//    //shadowTexDesc.MipLevels = 1;
//    //shadowTexDesc.ArraySize = NumCascades * 4; // 4 lights, 4 cascades each
//    //shadowTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
//    //shadowTexDesc.SampleDesc.Count = 1;
//    //shadowTexDesc.SampleDesc.Quality = 0;
//    //shadowTexDesc.Usage = D3D11_USAGE_DEFAULT;
//    //shadowTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
//    //shadowTexDesc.CPUAccessFlags = 0;
//    //shadowTexDesc.MiscFlags = 0;
//    //device_->CreateTexture2D(&shadowTexDesc, nullptr, &shadowMapTexture);
//
//    //// Create DSVs for each cascade (for first 4 lights)
//    //D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
//    //dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
//    //dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
//    //dsvDesc.Texture2DArray.MipSlice = 0;
//    //dsvDesc.Texture2DArray.ArraySize = 1; // One slice per DSV
//    //for (int i = 0; i < NumCascades; i++) {
//    //    dsvDesc.Texture2DArray.FirstArraySlice = i; // Each cascade gets its own slice
//    //    device_->CreateDepthStencilView(shadowMapTexture, &dsvDesc, &shadowMapDSVs[i]);
//    //}
//
//    //// Create SRV for the entire texture array
//    //D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//    //srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
//    //srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
//    //srvDesc.Texture2DArray.MostDetailedMip = 0;
//    //srvDesc.Texture2DArray.MipLevels = 1;
//    //srvDesc.Texture2DArray.FirstArraySlice = 0;
//    //srvDesc.Texture2DArray.ArraySize = NumCascades * 4;
//    //device_->CreateShaderResourceView(shadowMapTexture, &srvDesc, &shadowMapSRV);
//
//    //// Depth stencil state for shadow rendering
//    //D3D11_DEPTH_STENCIL_DESC dsDesc = {};
//    //dsDesc.DepthEnable = TRUE;
//    //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//    //dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//    //device_->CreateDepthStencilState(&dsDesc, &shadowDepthState);
//
//    Game::Initialize();
//}

//void KatamariGame::Draw()
//{
//    context_->ClearState();
//
//    context_->OMSetRenderTargets(1, &render_view_, depth_stencil_view_);
//
//    constexpr float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//    context_->ClearRenderTargetView(render_view_, color);
//    context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//    context_->PSSetSamplers(0, 1, &sampler_state_);
//    context_->PSSetSamplers(1, 1, &depth_sampler_state_);
//    context_->RSSetState(rast_state_);
//    if (isDebug) {
//        context_->VSSetShader(DataProcesser::GetVertexShader("spinny"), nullptr, 0);
//        context_->PSSetShader(DataProcesser::GetPixelShader("spinny"), nullptr, 0);
//    }
//    else {
//        context_->VSSetShader(DataProcesser::GetVertexShader("base"), nullptr, 0);
//        context_->PSSetShader(DataProcesser::GetPixelShader("base"), nullptr, 0);
//    }
//    for (auto c : components_)
//    {
//        c->Draw();
//    }
//}

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
            light.color = Vector3(1.0f, 0.0f, 1.0f);
            lightSpheres[i]->SetPosition(light.position);
            lightSpheres[i]->diffuseColor = light.color;
            break;
        }
    }
}

//void KatamariGame::UpdateObjectLights(GameComponent* obj)
//{
//
//    BaseComponent* baseComponent = dynamic_cast<BaseComponent*>(obj);
//
//    baseComponent->rotation.Normalize();
//    const Matrix world = Matrix::CreateScale(baseComponent->scale) * Matrix::CreateFromQuaternion(baseComponent->rotation) * Matrix::CreateTranslation(baseComponent->position);
//
//    BaseComponent::CBDataPerObject objData = {};
//    objData.worldViewProj = world * Camera->GetViewProj();
//    objData.world = world;
//    objData.WorldView = world * Camera->GetView();
//    objData.invTrWorld = (baseComponent->isSpinningFloor > 0.5f) ? Matrix::Identity : (Matrix::CreateScale(baseComponent->scale) * Matrix::CreateFromQuaternion(baseComponent->rotation)).Invert().Transpose();
//    objData.isSpinningFloor = baseComponent->isSpinningFloor;
//    objData.diffuseColor = baseComponent->diffuseColor;
//    objData.specularColor = baseComponent->specularColor;
//    objData.shininess = baseComponent->shininess;
//
//
//    // Find closest 10 lights
//    std::vector<std::pair<float, size_t>> distances;
//    Vector3 objPos = baseComponent->GetPosition();
//    for (size_t i = 0; i < pointLights.size(); i++) {
//        if (pointLights[i].active) {
//            float dist = (pointLights[i].position - objPos).Length();
//            if (dist < 100.0f) { // Only consider lights within radius
//                distances.push_back({ dist, i });
//            }
//        }
//    }
//    std::sort(distances.begin(), distances.end()); // Sort by distance
//
//    objData.numPointLights = std::min(int(distances.size()), 10);
//    for (int i = 0; i < objData.numPointLights; i++) {
//        const auto& light = pointLights[distances[i].second];
//        objData.pointLights[i].position = Vector4(light.position.x, light.position.y, light.position.z, 1.0f);
//        objData.pointLights[i].color = Vector4(light.color.x, light.color.y, light.color.z, light.intensity);
//    }
//
//    BaseComponent::CbDataCascade cascadeData = {};
//    auto tmp = GetDLight()->GetLightSpaceMatrices();
//    for (int i = 0; i < 5; ++i)
//    {
//        cascadeData.ViewProj[i] = tmp[i];
//    }
//    cascadeData.Distance = GetDLight()->GetShadowCascadeDistances();
//
//    context_->UpdateSubresource(baseComponent->objConstantBuffer, 0, nullptr, &objData, 0, 0);
//    context_->UpdateSubresource(baseComponent->cascadeConstantBuffer, 0, nullptr, &cascadeData, 0, 0);
//}
#pragma once﻿
#include "KatamariGame.h"
#include "../Components/GridComponent.h"
#include "KatamariTrash.h"
#include "../Components/RectangleComponent.h"
#include "KatamariBall.h"

using namespace DirectX;
using namespace SimpleMath;

KatamariGame::KatamariGame() : Game(L"Katamari Game", 800, 800), cameraController(this)
{
    srand(static_cast<unsigned>(time(nullptr)));

    ball = new KatamariBall(this);
    ball->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    components_.push_back(ball);

    RectangleComponent* quad = new RectangleComponent(this, L"Textures/pavement.dds");
    quad->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::Left, XM_PI / 2.0f));
    quad->SetScale(Vector3::One * 1000.0f);
    components_.push_back(quad);

    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* cat = new KatamariTrash(this, "Models/maxwell.obj", L"Textures/dingus_nowhiskers.dds", 1.3f, Vector3(0.0f, 0.0f, 0.0f));
        cat->SetScale(Vector3(0.0009f, 0.0009f, 0.0009f));
        cat->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        cat->collision.Radius = 0.4f;
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
        gabriel->collision.Radius = 0.1f;
        components_.push_back(gabriel);
        furniture.push_back(gabriel);
    }

    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* duck = new KatamariTrash(this, "Models/melon.obj", L"Textures/melon.dds", 2.0f, Vector3(0.0f, 0.0f, 0.0f));
        duck->SetScale(Vector3(10.f,10.f, 10.f));
        duck->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        duck->collision.Radius = 1.f;
        components_.push_back(duck);
        furniture.push_back(duck);
    }
    for (int i = 0; i < 10; ++i)
    {
        KatamariTrash* duck = new KatamariTrash(this, "Models/cucumber.obj", L"Textures/cucumber.dds", 0.9f, Vector3(0.0f, 0.0f, 0.0f));
        duck->SetScale(Vector3(7.f, 7.f, 7.f));
        duck->SetPosition(Vector3(static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f, 0.0f, static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f));
        duck->collision.Radius = 0.8f;
        components_.push_back(duck);
        furniture.push_back(duck);
    }

    KatamariTrash* sofa = new KatamariTrash(this, "Models/bread.obj", L"Textures/bread.dds", 3.0f, Vector3(0.0f, 1.0f, 0.0f));
    sofa->SetScale(Vector3(50.0f, 50.0f, 50.0f));
    sofa->SetPosition(Vector3(30.0f, 0.0f, 10.0f));
    sofa->collision.Radius = 2.5f;
    components_.push_back(sofa);
    furniture.push_back(sofa);
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
    Game::Update();
}
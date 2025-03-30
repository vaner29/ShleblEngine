#include "CameraController.h"
#include "SpaceGame.h"
#include "../KatamariComponents/KatamariGame.h"

using namespace DirectX;
using namespace SimpleMath;

CameraController::CameraController(Game* g) : up(Vector3::Up), game(g), OrbitMode(true), speed(0.2f)
{
    sGame = dynamic_cast<SpaceGame*>(g);
    kGame = dynamic_cast<KatamariGame*>(g);
    relativePos = game->Camera->Position;
}

void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    if (OrbitMode)
    {
        if (game->input_dev_->IsKeyDown(Keys::LeftButton))
        {
            // Get the right vector relative to the camera's current orientation
            auto right = relativePos.Cross(up);
            auto qua = Quaternion::CreateFromAxisAngle(up, 0.005f * game->input_dev_->MouseOffset.x) * Quaternion::CreateFromAxisAngle(right, -0.005f * game->input_dev_->MouseOffset.y);
            // Apply the combined rotation to the relativePos and up vectors
            relativePos = XMVector3Transform(relativePos, Matrix::CreateFromQuaternion(qua));
            up = XMVector3Transform(Vector3::Up, Matrix::CreateFromQuaternion(qua));
            // Explicitly enforce no roll by aligning the up vector with the world's up axis
            up = Vector3::Up;
            game->Camera->Up = up;
        }
        // Handle zooming
        relativePos *= 1 - 0.001f * game->input_dev_->MouseWheelDelta;
        if (followShip) {
            Vector3 forward = game->Camera->Target - game->Camera->Position;
            forward.Normalize();
            Vector3 up = game->Camera->Up;
            up.Normalize();

            // Calculate yaw (rotation around the Y-axis)
            yaw = atan2f(forward.x, forward.z);

            // Calculate pitch (rotation around the X-axis)
            pitch = asinf(-forward.y);
        }
    }
    else
    {
        if (game->input_dev_->IsKeyDown(Keys::LeftButton))
        {
            yaw -= 0.004f * game->input_dev_->MouseOffset.x;
            while (yaw < -XM_2PI)
                yaw += XM_2PI;
            pitch -= 0.004f * game->input_dev_->MouseOffset.y;
            game->Camera->Up = XMVector4Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            game->Camera->Target = game->Camera->Position;
            game->Camera->Target += XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        }
    }
}

void CameraController::Update()
{
    if (game->input_dev_->IsKeyDown(Keys::F1))
        game->Camera->IsOrthographic = false;
    if (game->input_dev_->IsKeyDown(Keys::F2))
        game->Camera->IsOrthographic = true;

    //if (OrbitMode && (game->input_dev_->IsKeyDown(Keys::W) || game->input_dev_->IsKeyDown(Keys::A) || game->input_dev_->IsKeyDown(Keys::S)
    //    || game->input_dev_->IsKeyDown(Keys::D) || game->input_dev_->IsKeyDown(Keys::E) || game->input_dev_->IsKeyDown(Keys::Z)))
    //{
    //    OrbitMode = false;
    //    followShip = false;a
    //    auto q = Quaternion::LookRotation(game->Camera->Target - game->Camera->Position, game->Camera->Up);
    //    auto v = q.ToEuler();
    //    yaw = v.y;
    //    pitch = 0.0f;
    //    targetBody = nullptr;
    //}
    if (game->input_dev_->IsKeyDown(Keys::D0))
    {
        OrbitMode = true;
        followShip = false;
        targetBall = kGame->ball;
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    //if (game->input_dev_->IsKeyDown(Keys::D1))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["mercury"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}
    //if (game->input_dev_->IsKeyDown(Keys::D2))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["venus"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}
    //if (game->input_dev_->IsKeyDown(Keys::D3))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["earth"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}
    //if (game->input_dev_->IsKeyDown(Keys::D4))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["moon"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}
    //if (game->input_dev_->IsKeyDown(Keys::D5))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["moon2"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}

    //if (game->input_dev_->IsKeyDown(Keys::D6))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["moon3"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}

    //if (game->input_dev_->IsKeyDown(Keys::D7))
    //{
    //    OrbitMode = true;
    //    followShip = false;
    //    targetBody = sGame->celestialBodies["moon4"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}

    //if (game->input_dev_->IsKeyDown(Keys::D9))
    //{
    //    OrbitMode = true;
    //    followShip = true;
    //    targetBody = sGame->celestialBodies["ship"];
    //    game->Camera->Position = Vector3::Forward * 20.0f;
    //    game->Camera->Up = Vector3::Up;
    //    up = Vector3::Up;
    //}

    if (OrbitMode && targetBall)
    {
        game->Camera->Target = targetBall->GetPosition();
        game->Camera->Position = targetBall->GetPosition() + relativePos;
    }

    if (OrbitMode && !targetBall)
    {
        game->Camera->Target = Vector3::Zero;
        game->Camera->Position = relativePos;
    }

    if (!OrbitMode)
    {
        if (game->input_dev_->IsKeyDown(Keys::W))
        {
            Vector3 tmp = XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->input_dev_->IsKeyDown(Keys::S))
        {
            Vector3 tmp = XMVector4Transform(Vector3::Backward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->input_dev_->IsKeyDown(Keys::A))
        {
            Vector3 tmp = XMVector4Transform(Vector3::Left, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->input_dev_->IsKeyDown(Keys::D))
        {
            Vector3 tmp = XMVector4Transform(Vector3::Right, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->input_dev_->IsKeyDown(Keys::E))
        {
            game->Camera->Position += speed * Vector3::Up;
        }
        if (game->input_dev_->IsKeyDown(Keys::Z))
        {
            game->Camera->Position += speed * Vector3::Down;
        }
        game->Camera->Up = XMVector4Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        game->Camera->Target = game->Camera->Position;
        game->Camera->Target += XMVector4Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
    }
}


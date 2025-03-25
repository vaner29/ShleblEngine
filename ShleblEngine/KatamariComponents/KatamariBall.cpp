#include "KatamariBall.h"
#include "../Components/Game.h"
#include "KatamariGame.h"
#include "KatamariTrash.h"

using namespace DirectX;
using namespace SimpleMath;

void KatamariBall::UpdateSize(float absorbedSize)
{
    float tmp = sqrtf(gameSize * gameSize + absorbedSize * absorbedSize * 0.3f);
    collision.Radius = tmp;
    position.y = tmp;
    rotationMaxSpeed = 0.1f / (tmp * tmp);
    if (rotationMaxSpeed < 0.005f)
        rotationMaxSpeed = 0.005f;
    moveMaxSpeed = 8.0f * sqrtf(tmp);
    rotationDrag = 0.1f + 0.06f / sqrtf(tmp);
    SetScale(DirectX::SimpleMath::Vector3::One * (tmp / radius));
    gameSize = tmp;
}

KatamariBall::KatamariBall(Game* game) : SphereComponent(game, 1.0f, 32, 32, L"Textures/slime.dds"), rotationDrag(0.14f), rotationMaxSpeed(0.1f), moveMaxSpeed(8.0f), moveDrag(5.0f), savedRot(Quaternion::Identity),
velocity(Vector3::Zero), collision(position, 1.0f), gameSize(1.0f)
{
    kGame = dynamic_cast<KatamariGame*>(game);
}

KatamariBall::~KatamariBall()
{
}

void KatamariBall::Initialize()
{
    SphereComponent::Initialize();
}

void KatamariBall::Draw()
{
    SphereComponent::Draw();
}

void KatamariBall::Update()
{
    collision.Center = position;
    for (auto furn : kGame->furniture)
    {
        if (collision.Intersects(furn->collision) && !furn->isPickedUp && gameSize > furn->gameSize)
        {
            furn->isPickedUp = true;
            furn->kb = this;
            furn->initRelPos = furn->GetPosition() - GetPosition();
            rotation.Inverse(furn->invKbRot);
            UpdateSize(furn->gameSize);
        }
    }

    savedRot.RotateTowards(Quaternion::Identity, rotationDrag * game->delta_time_);
    rotation *= savedRot;
    velocity *= 1.0f - moveDrag * game->delta_time_;

    SphereComponent::Update();

    position += velocity * game->delta_time_;
}

void KatamariBall::DestroyResources()
{
    SphereComponent::DestroyResources();
}

void KatamariBall::SetDirection(Vector3 dir)
{
    Vector3 tmp = Vector3(dir.x, 0.0f, dir.z);
    tmp.Normalize();
    Quaternion q = Quaternion::CreateFromAxisAngle(tmp.Cross(Vector3::Up), -rotationMaxSpeed);
    float f = Quaternion::Angle(Quaternion::Identity, savedRot) / 0.1f;
    savedRot *= Quaternion::Lerp(q, Quaternion::Identity, f);
    velocity = tmp * moveMaxSpeed;
}

void KatamariBall::SetPosition(DirectX::SimpleMath::Vector3 p)
{
    SphereComponent::SetPosition(p);
}
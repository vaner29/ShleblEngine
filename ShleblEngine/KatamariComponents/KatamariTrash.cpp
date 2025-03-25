#include "KatamariTrash.h"

#include "KatamariBall.h"

using namespace DirectX;
using namespace SimpleMath;

KatamariTrash::KatamariTrash(Game* g, const std::string fileNameModel, const wchar_t* fileNameTexture, float katSize, Vector3 collOffset) :
    ObjectComponent(g, fileNameModel, fileNameTexture), isPickedUp(false), collision(position + collOffset, katSize), originCollisionOffset(collOffset), gameSize(katSize)
{
    colorModePS = false;
}

void KatamariTrash::Update()
{
    ObjectComponent::Update();
    if (isPickedUp)
    {
        if (kb)
        {
            SetPosition(kb->GetPosition() + Vector3::Transform(initRelPos, invKbRot * kb->GetRotation()));
            SetRotation(invKbRot * kb->GetRotation());
        }
    }
}

void KatamariTrash::SetPosition(Vector3 p)
{
    collision.Center = p + originCollisionOffset;
    ObjectComponent::SetPosition(p);
}
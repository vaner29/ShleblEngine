#include "ObjectComponent.h"
#include <iostream>
#include "../Tools/DataProcesser.h"

using namespace DirectX;
using namespace SimpleMath;

ObjectComponent::ObjectComponent(Game* g, const std::string fileNameModel, const wchar_t* fileNameTexture) :
    BaseComponent(g), fNameModel(fileNameModel), isPickedUp(false)
{
    textureFileName_ = fileNameTexture;
}

void ObjectComponent::Initialize()
{
    points_ = DataProcesser::GetPoints(fNameModel);
    indices_ = DataProcesser::GetIndices(fNameModel);

    BaseComponent::Initialize();
}

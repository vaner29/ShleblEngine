#include "LightVolume.h"
#include "../Tools/DataProcesser.h"
#include "Game.h"

CD3D11_RASTERIZER_DESC LightVolume::CreateRasterizerStateDesc()
{
    auto rs = SphereComponent::CreateRasterizerStateDesc();
    rs.CullMode = D3D11_CULL_FRONT;
    return rs;
}

LightVolume::LightVolume(Game* g) : SphereComponent(g, 1.0f, 16, 16, L"Textures/duck.dds")
{
    isShadowCasting_ = false;
}

void LightVolume::SetSize(float size)
{
    SphereComponent::SetScale(DirectX::SimpleMath::Vector3(size, size, size));
}

void LightVolume::Draw()
{
    game->context_->RSSetState(rastState_); //TODO: Solve for moving that somewhere

    game->context_->IASetInputLayout(layout_);
    game->context_->IASetPrimitiveTopology(topologyType);
    game->context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
    game->context_->IASetVertexBuffers(0, 1, &vertex_buffer_, strides, offsets);
    game->context_->VSSetShader(DataProcesser::GetVertexShader("lightpassvolume"), nullptr, 0);
    game->context_->VSSetConstantBuffers(0, 1, &objConstantBuffer);
    game->context_->PSSetShader(DataProcesser::GetPixelShader("lightpassvolume"), nullptr, 0);

    game->context_->DrawIndexed(indices_.size(), 0, 0);
}

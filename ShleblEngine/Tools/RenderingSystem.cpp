#include "RenderingSystem.h"
#include "../Components/BaseComponent.h"
#include "../Components/Camera.h"
#include "DataProcesser.h"

RenderingSystem::RenderingSystem(ID3D11Device* device, int width, int height) : device(device) {
    device->GetImmediateContext(&context);
    gbuffer = new Gbuffer(device, width, height);
    quadVS = DataProcesser::GetVertexShader("quad");
    geometryPS = DataProcesser::GetPixelShader("geometry");
}

RenderingSystem::~RenderingSystem() {
    delete gbuffer;
}

void RenderingSystem::RenderScene(const std::vector<GameComponent*>& components, const Camera& camera, const std::vector<Light*>& lights, ID3D11RenderTargetView* backBuffer, ID3D11DepthStencilView* depthStencil) {
    context->ClearState();
    gbuffer->Clear(context.Get());
    GeometryPass(components, camera);
    LightingPass(lights, camera, backBuffer);
    context->OMSetRenderTargets(0, nullptr, nullptr);
}

void RenderingSystem::GeometryPass(const std::vector<GameComponent*>& components, const Camera& camera) {
    gbuffer->BindForWriting(context.Get());
    context->VSSetShader(DataProcesser::GetVertexShader("geometry"), nullptr, 0);
    context->PSSetShader(geometryPS, nullptr, 0);
    for (auto* component : components) {
        BaseComponent* baseComp = dynamic_cast<BaseComponent*>(component);
        if (baseComp) {
            baseComp->DrawGeometryPass(context.Get());
        }
    }
}

void RenderingSystem::LightingPass(const std::vector<Light*>& lights, const Camera& camera, ID3D11RenderTargetView* backBuffer) {
    context->OMSetRenderTargets(1, &backBuffer, nullptr);
    gbuffer->BindForReading(context.Get());
    context->VSSetShader(quadVS, nullptr, 0);
    for (auto* light : lights) {
        if (light->IsInFrustum(camera)) {
            light->Apply(context.Get());
            context->Draw(6, 0); // Draw full-screen quad
        }
    }
}
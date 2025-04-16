#pragma once
#include "../Components/GBuffer.h"
#include "../Components/Light.h"
#include <vector>
#include <wrl.h>

class GameComponent;
class Camera;

class RenderingSystem {
public:
    RenderingSystem(ID3D11Device* device, int width, int height);
    ~RenderingSystem();
    void RenderScene(const std::vector<GameComponent*>& components, const Camera& camera, const std::vector<Light*>& lights, ID3D11RenderTargetView* backBuffer, ID3D11DepthStencilView* depthStencil);

private:
    void GeometryPass(const std::vector<GameComponent*>& components, const Camera& camera);
    void LightingPass(const std::vector<Light*>& lights, const Camera& camera, ID3D11RenderTargetView* backBuffer);

    ID3D11Device* device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Gbuffer* gbuffer;
    ID3D11VertexShader* quadVS;
    ID3D11PixelShader* geometryPS;
};
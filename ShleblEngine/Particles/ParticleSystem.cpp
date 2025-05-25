#include "ParticleSystem.h"

#include "../Components/BaseComponent.h"
#include "../Components/Game.h"
#include "../Tools/DataProcesser.h"
#include <type_traits>

using namespace DirectX;
using namespace SimpleMath;

static D3D_SHADER_MACRO* GetMacros(const ParticleSystem::ComputeFlags& flag)
{
    // Allocate array for up to 4 macros (3 flags + 1 null terminator)
    D3D_SHADER_MACRO* ShaderMacros = new D3D_SHADER_MACRO[4]();
    int macrosCount = 0;

    using FlagType = std::underlying_type_t<ParticleSystem::ComputeFlags>;
    FlagType flagValue = static_cast<FlagType>(flag);

    if (flagValue & static_cast<FlagType>(ParticleSystem::ComputeFlags::INJECTION))
    {
        ShaderMacros[macrosCount++] = { "INJECTION", "1" };
    }

    if (flagValue & static_cast<FlagType>(ParticleSystem::ComputeFlags::SIMULATION))
    {
        ShaderMacros[macrosCount++] = { "SIMULATION", "1" };
    }

    if (flagValue & static_cast<FlagType>(ParticleSystem::ComputeFlags::ADD_GRAVITY))
    {
        ShaderMacros[macrosCount++] = { "ADD_GRAVITY", "1" };
    }

    // Last entry is null-terminated as required by D3DCompileFromFile
    ShaderMacros[macrosCount] = { nullptr, nullptr };

    return ShaderMacros;
}

static float RandomFloatInRange(float low, float high)
{
    return (high - low) * static_cast<float>(rand()) / RAND_MAX + low;
}

static Vector4 RandomVectorInRange(Vector3 range)
{
    Vector4 res = Vector4::Zero;
    res.x = RandomFloatInRange(-range.x, range.x);
    res.y = RandomFloatInRange(-range.y, range.y);
    res.z = RandomFloatInRange(-range.z, range.z);
    return res;
}

ParticleSystem::ParticleSystem(Game* g) : GameComponent(g)
{
    Width = 1.0f;
    Height = 1.0f;
    Length = 1.0f;
    Position = DirectX::SimpleMath::Vector3(0, 2, 0);

    GravityAffected = true;

    EmitterSettings.Enabled = true;
    EmitterSettings.ParticlesPerSecond = 100.0f;
    EmitterSettings.Color0 = Vector4::One;
    EmitterSettings.Velocity = Vector3::Zero;
    EmitterSettings.NudgeColorLum = true;
    EmitterSettings.NudgeColorHue = false;
    EmitterSettings.Size0 = 0.05f;
    EmitterSettings.Size1 = 0.02f;
    EmitterSettings.LifeTime = 1.0f;
    EmitterSettings.NudgeLifeTime = false;
    EmitterSettings.DynamicVelocityX = Vector2::Zero;
    EmitterSettings.DynamicVelocityY = Vector2::Zero;
    EmitterSettings.DynamicVelocityZ = Vector2::Zero;

    TextureFileName = L"Textures/car.dds";

    GroundLevel = -5;
}

void ParticleSystem::GetGroupSize(int particleCount, int& groupSizeX, int& groupSizeY)
{
    int numGroups = (particleCount % 256 != 0) ? ((particleCount / 256) + 1) : (particleCount / 256);
    auto root = std::sqrt(static_cast<double>(numGroups));
    root = std::ceil(root);
    groupSizeX = static_cast<int>(root);
    groupSizeY = groupSizeX;
}

void ParticleSystem::Initialize()
{
    LoadShaders();
    CreateBuffers();

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = true;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    game->device_->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

    CD3D11_BLEND_DESC blendDesc = {};
    blendDesc.IndependentBlendEnable = false;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    game->device_->CreateBlendState(&blendDesc, blendState.GetAddressOf());

    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.StencilEnable = false;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.StencilReadMask = 0x00;
    depthStencilDesc.StencilWriteMask = 0x00;

    game->device_->CreateDepthStencilState(&depthStencilDesc, depthState.GetAddressOf());

    D3D11_SAMPLER_DESC samplerStateDesc = {};
    samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.MinLOD = 0.0f;
    samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

    game->device_->CreateSamplerState(&samplerStateDesc, samplerState.GetAddressOf());
}

void ParticleSystem::Update()
{
    if (EmitterSettings.Enabled)
    {
        emitterTimeSave += game->delta_time_;
        while (emitterTimeSave / (1.0f / EmitterSettings.ParticlesPerSecond) >= 1)
        {
            Particle p = {};
            p.LifeTime = EmitterSettings.LifeTime + (EmitterSettings.NudgeLifeTime ? RandomFloatInRange(-0.5f, 0.5f) : 0.0f);
            p.Velocity = Vector4(EmitterSettings.Velocity.x, EmitterSettings.Velocity.y, EmitterSettings.Velocity.z, 0.0f) + (EmitterSettings.NudgeVelocity ? RandomVectorInRange(Vector3(0.05f, 0.05f, 0.05f)) : Vector4::Zero);
            if (EmitterSettings.DynamicVelocityX != Vector2::Zero) {
                p.Velocity = Vector4(p.Velocity.x + RandomFloatInRange(EmitterSettings.DynamicVelocityX.x, EmitterSettings.DynamicVelocityX.y), p.Velocity.y, p.Velocity.z, p.Velocity.w);
            }
            if (EmitterSettings.DynamicVelocityY != Vector2::Zero) {
                p.Velocity = Vector4(p.Velocity.x, p.Velocity.y + RandomFloatInRange(EmitterSettings.DynamicVelocityY.x, EmitterSettings.DynamicVelocityY.y), p.Velocity.z, p.Velocity.w);
            }
            if (EmitterSettings.DynamicVelocityZ != Vector2::Zero) {
                p.Velocity = Vector4(p.Velocity.x, p.Velocity.y, p.Velocity.z + RandomFloatInRange(EmitterSettings.DynamicVelocityZ.x, EmitterSettings.DynamicVelocityZ.y), p.Velocity.w);
            }
            p.Color0 = (EmitterSettings.Color0 + (EmitterSettings.NudgeColorHue ? RandomVectorInRange(Vector3(0.05f, 0.05f, 0.05f)) : Vector4::Zero)) * (EmitterSettings.NudgeColorLum ? RandomFloatInRange(0.9f, 1.1f) : 1.0f);
            p.Size0Size1 = Vector2(EmitterSettings.Size0, EmitterSettings.Size1);
            p.Position = Vector4(Length * RandomFloatInRange(-1.0f, 1.0f), Height * RandomFloatInRange(-1.0f, 1.0f), Width * RandomFloatInRange(-1.0f, 1.0f), 0.0f);
            p.Position.Normalize();
            p.Position.x *= RandomFloatInRange(0.0, Width / 2.0f);
            p.Position.y *= RandomFloatInRange(0.0, Height / 2.0f);
            p.Position.z *= RandomFloatInRange(0.0, Length / 2.0f);
            p.Position.w = 1.0f;
            p.MaxLifeTime = p.LifeTime;
            AddParticle(p);
            emitterTimeSave -= 1.0f / EmitterSettings.ParticlesPerSecond;
        }
    }

    int groupSizeX, groupSizeY;
    GetGroupSize(ParticlesCount, groupSizeX, groupSizeY);

    constData.World = Matrix::CreateTranslation(Position);
    constData.View = game->Camera->GetView();
    constData.Proj = game->Camera->GetProj();
    constData.DeltaTimeMaxParticlesGroupdimGround = Vector4(game->delta_time_, ParticlesCount, groupSizeY, GroundLevel);

    game->context_->UpdateSubresource(constBuf.Get(), 0, nullptr, &constData, 0, 0);
    game->context_->CSSetConstantBuffers(0, 1, constBuf.GetAddressOf());

    constexpr UINT counterKeepValue = -1;
    constexpr UINT counterZero = 0;

    game->context_->CSSetUnorderedAccessViews(0, 1, uavSrc.GetAddressOf(), &counterKeepValue);
    game->context_->CSSetUnorderedAccessViews(1, 1, uavDst.GetAddressOf(), &counterZero);

    if (GravityAffected)
    {
        using FlagType = std::underlying_type_t<ParticleSystem::ComputeFlags>;
        game->context_->CSSetShader(ComputeShaders[static_cast<ComputeFlags>(
            static_cast<FlagType>(ComputeFlags::SIMULATION) |
            static_cast<FlagType>(ComputeFlags::ADD_GRAVITY))].Get(), nullptr, 0);
    }
    else
    {
        game->context_->CSSetShader(ComputeShaders[ComputeFlags::SIMULATION].Get(), nullptr, 0);
    }

    if (groupSizeX > 0)
    {
        game->context_->Dispatch(groupSizeX, groupSizeY, 1);
    }

    if (InjectionCount > 0)
    {
        int injSizeX, injSizeY;
        GetGroupSize(InjectionCount, injSizeX, injSizeY);

        constData.DeltaTimeMaxParticlesGroupdimGround = Vector4(game->delta_time_, static_cast<float>(InjectionCount), static_cast<float>(injSizeY), GroundLevel);

        game->context_->UpdateSubresource(constBuf.Get(), 0, nullptr, &constData, 0, 0);
        game->context_->CSSetConstantBuffers(0, 1, constBuf.GetAddressOf());

        game->context_->UpdateSubresource(injectionBuf.Get(), 0, nullptr, injectionParticles, 0, 0);

        game->context_->CSSetUnorderedAccessViews(0, 1, injUav.GetAddressOf(), &InjectionCount);
        game->context_->CSSetShader(ComputeShaders[ComputeFlags::INJECTION].Get(), nullptr, 0);

        game->context_->Dispatch(injSizeX, injSizeY, 1);

        InjectionCount = 0;
    }

    ID3D11UnorderedAccessView* nuPtr = nullptr;
    game->context_->CSSetUnorderedAccessViews(0, 1, &nuPtr, &counterZero);
    game->context_->CSSetUnorderedAccessViews(1, 1, &nuPtr, &counterZero);

    game->context_->CopyStructureCount(countBuf.Get(), 0, uavDst.Get());

    D3D11_MAPPED_SUBRESOURCE sr;
    game->context_->Map(countBuf.Get(), 0, D3D11_MAP_READ, 0, &sr);
    UINT* data = reinterpret_cast<UINT*>(sr.pData);
    ParticlesCount = data[0];
    game->context_->Unmap(countBuf.Get(), 0);

    SwapBuffers();
}

void ParticleSystem::Draw()
{
    game->context_->ClearState();

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> oldRastState = nullptr;
    game->context_->RSGetState(oldRastState.GetAddressOf());
    game->context_->RSSetState(rastState.Get());

    Microsoft::WRL::ComPtr<ID3D11BlendState> oldBlendState = nullptr;
    UINT oldBlendMask = 0;
    float oldBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    game->context_->OMGetBlendState(oldBlendState.GetAddressOf(), oldBlendFactor, &oldBlendMask);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    game->context_->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> oldDepthState = nullptr;
    UINT oldStencilRef = 0;
    game->context_->OMGetDepthStencilState(oldDepthState.GetAddressOf(), &oldStencilRef);
    game->context_->OMSetDepthStencilState(depthState.Get(), 0);
    game->context_->OMSetRenderTargets(1, game->GetMainRTV(), game->GetMainDSV());

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(game->display_->client_width_);
    viewport.Height = static_cast<float>(game->display_->client_height_);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    game->context_->RSSetViewports(1, &viewport);

    game->context_->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    game->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    game->context_->VSSetShader(vertexShader.Get(), nullptr, 0);
    game->context_->VSSetShaderResources(0, 1, srvSrc.GetAddressOf());
    game->context_->VSSetConstantBuffers(0, 1, constBuf.GetAddressOf());
    if (IsTextured)
    {
        game->context_->PSSetShader(pixelShaderTex.Get(), nullptr, 0);
        const auto texture = DataProcesser::GetTextureView(TextureFileName);
        game->context_->PSSetShaderResources(0, 1, &texture);
        game->context_->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    }
    else
    {
        game->context_->PSSetShader(pixelShader.Get(), nullptr, 0);
    }

    game->context_->DrawIndexedInstancedIndirect(bufArgs.Get(), 0);

    game->context_->ClearState();

    game->context_->RSSetState(oldRastState.Get());
    game->context_->OMSetBlendState(oldBlendState.Get(), oldBlendFactor, oldBlendMask);
    game->context_->OMSetDepthStencilState(oldDepthState.Get(), oldStencilRef);
}

void ParticleSystem::DestroyResources()
{
}

void ParticleSystem::LoadShaders()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexBC;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;
    Microsoft::WRL::ComPtr<ID3DBlob> errorCode;

    auto res = D3DCompileFromFile(L"./Shaders/Particles.hlsl",
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        vertexBC.GetAddressOf(),
        errorCode.GetAddressOf());

    res = D3DCompileFromFile(L"./Shaders/Particles.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        pixelBC.GetAddressOf(),
        errorCode.GetAddressOf());

    game->device_->CreateVertexShader(vertexBC.Get()->GetBufferPointer(),
        vertexBC.Get()->GetBufferSize(), nullptr, vertexShader.GetAddressOf());

    game->device_->CreatePixelShader(pixelBC.Get()->GetBufferPointer(),
        pixelBC.Get()->GetBufferSize(), nullptr, pixelShader.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3DBlob> pixelTexBC;

    res = D3DCompileFromFile(L"./Shaders/ParticlesTex.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        pixelTexBC.GetAddressOf(),
        errorCode.GetAddressOf());

    game->device_->CreatePixelShader(pixelTexBC.Get()->GetBufferPointer(),
        pixelTexBC.Get()->GetBufferSize(), nullptr, pixelShaderTex.GetAddressOf());

    using FlagType = std::underlying_type_t<ParticleSystem::ComputeFlags>;
    std::vector<ComputeFlags> flags = {
        ComputeFlags::INJECTION,
        ComputeFlags::SIMULATION,
        static_cast<ComputeFlags>(static_cast<FlagType>(ComputeFlags::SIMULATION) |
                                  static_cast<FlagType>(ComputeFlags::ADD_GRAVITY)),
    };

    for (auto& flag : flags)
    {
        D3D_SHADER_MACRO* macros = GetMacros(flag);
        Microsoft::WRL::ComPtr<ID3DBlob> computeBC;
        res = D3DCompileFromFile(L"./Shaders/ParticlesSim.hlsl",
            macros,
            nullptr,
            "CSMain",
            "cs_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            computeBC.GetAddressOf(),
            errorCode.GetAddressOf());

        Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader;
        game->device_->CreateComputeShader(computeBC.Get()->GetBufferPointer(),
            computeBC.Get()->GetBufferSize(), nullptr, computeShader.GetAddressOf());

        ComputeShaders.emplace(flag, computeShader);
    }
}

void ParticleSystem::CreateBuffers()
{
    D3D11_BUFFER_DESC constBufDesc = {};
    constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufDesc.Usage = D3D11_USAGE_DEFAULT;
    constBufDesc.MiscFlags = 0;
    constBufDesc.CPUAccessFlags = 0;
    constBufDesc.ByteWidth = sizeof(ConstData);

    game->device_->CreateBuffer(&constBufDesc, nullptr, constBuf.GetAddressOf());

    D3D11_BUFFER_DESC particleBufDesc = {};
    particleBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    particleBufDesc.Usage = D3D11_USAGE_DEFAULT;
    particleBufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    particleBufDesc.CPUAccessFlags = 0;
    particleBufDesc.StructureByteStride = sizeof(Particle);
    particleBufDesc.ByteWidth = MaxParticlesCount * sizeof(Particle);


    game->device_->CreateBuffer(&particleBufDesc, nullptr, bufFirst.GetAddressOf());
    game->device_->CreateBuffer(&particleBufDesc, nullptr, bufSecond.GetAddressOf());

    game->device_->CreateShaderResourceView(bufFirst.Get(), nullptr, srvFirst.GetAddressOf());
    game->device_->CreateShaderResourceView(bufSecond.Get(), nullptr, srvSecond.GetAddressOf());

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer = D3D11_BUFFER_UAV{
        0,
        MaxParticlesCount,
        D3D11_BUFFER_UAV_FLAG_APPEND
    };

    game->device_->CreateUnorderedAccessView(bufFirst.Get(), &uavDesc, uavFirst.GetAddressOf());
    game->device_->CreateUnorderedAccessView(bufSecond.Get(), &uavDesc, uavSecond.GetAddressOf());

    srvSrc = srvFirst;
    uavSrc = uavFirst;
    srvDst = srvSecond;
    uavDst = uavSecond;

    ID3D11UnorderedAccessView* nuPtr = nullptr;
    game->context_->CSSetUnorderedAccessViews(0, 1, uavSrc.GetAddressOf(), &MaxParticlesCount);
    game->context_->CSSetUnorderedAccessViews(0, 1, &nuPtr, nullptr);

    D3D11_BUFFER_DESC countBufDesc = {};
    countBufDesc.BindFlags = 0;
    countBufDesc.Usage = D3D11_USAGE_STAGING;
    countBufDesc.MiscFlags = 0;
    countBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    countBufDesc.StructureByteStride = 0;
    countBufDesc.ByteWidth = 4;

    game->device_->CreateBuffer(&countBufDesc, nullptr, countBuf.GetAddressOf());

    D3D11_BUFFER_DESC injectionBufDesc = {};
    injectionBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    injectionBufDesc.Usage = D3D11_USAGE_DEFAULT;
    injectionBufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    injectionBufDesc.CPUAccessFlags = 0;
    injectionBufDesc.StructureByteStride = sizeof(Particle);
    injectionBufDesc.ByteWidth = MaxParticlesInjectionCount * sizeof(Particle);

    game->device_->CreateBuffer(&injectionBufDesc, nullptr, injectionBuf.GetAddressOf());

    D3D11_UNORDERED_ACCESS_VIEW_DESC injUavDesc = {};
    injUavDesc.Format = DXGI_FORMAT_UNKNOWN;
    injUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    injUavDesc.Buffer = D3D11_BUFFER_UAV{
        0,
        MaxParticlesInjectionCount,
        D3D11_BUFFER_UAV_FLAG_APPEND
    };

    game->device_->CreateUnorderedAccessView(injectionBuf.Get(), &injUavDesc, injUav.GetAddressOf());

    D3D11_BUFFER_DESC indexBufDesc;
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(UINT) * MaxParticlesCount * 6;

    D3D11_SUBRESOURCE_DATA indexData;
    UINT* indices = new UINT[MaxParticlesCount * 6];
    for (UINT i = 0; i < MaxParticlesCount; ++i)
    {
        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;

        indices[i * 6 + 3] = i * 4 + 1;
        indices[i * 6 + 4] = i * 4 + 0;
        indices[i * 6 + 5] = i * 4 + 3;
    }
    indexData.pSysMem = &indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->device_->CreateBuffer(&indexBufDesc, &indexData, indexBuffer.GetAddressOf());

    delete[] indices;

    D3D11_SUBRESOURCE_DATA argsData;
    DrawArgs args = {};
    args.IndexCountPerInstance = MaxParticlesCount * 6;
    args.InstanceCount = 1;
    argsData.pSysMem = &args;
    argsData.SysMemPitch = 0;
    argsData.SysMemSlicePitch = 0;

    D3D11_BUFFER_DESC argsBufDesc = {};
    argsBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    argsBufDesc.Usage = D3D11_USAGE_DEFAULT;
    argsBufDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
    argsBufDesc.CPUAccessFlags = 0;
    argsBufDesc.StructureByteStride = sizeof(DrawArgs);
    argsBufDesc.ByteWidth = sizeof(DrawArgs);

    auto res = game->device_->CreateBuffer(&argsBufDesc, &argsData, bufArgs.GetAddressOf());
}

void ParticleSystem::AddParticle(const Particle& p)
{
    injectionParticles[InjectionCount++] = p;
}

void ParticleSystem::SwapBuffers()
{
    auto tmpSrv = srvSrc;
    auto tmpUav = uavSrc;

    srvSrc = srvDst;
    uavSrc = uavDst;

    srvDst = tmpSrv;
    uavDst = tmpUav;
}
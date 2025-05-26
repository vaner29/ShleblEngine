#pragma once
#include <map>
#include <wrl/client.h>

#include "../Components/GameComponent.h"
#include "d3d11.h"

class ParticleSystem : public GameComponent
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> bufFirst;
    Microsoft::WRL::ComPtr<ID3D11Buffer> bufSecond;
    Microsoft::WRL::ComPtr<ID3D11Buffer> countBuf;
    Microsoft::WRL::ComPtr<ID3D11Buffer> injectionBuf;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constBuf;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constSortBuf;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> sortBuffer;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvFirst;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvSecond;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvSrc;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvDst;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvSort;

    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uavFirst;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uavSecond;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uavSrc;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uavDst;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uavSort;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> injUav;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

    float emitterTimeSave = 0;

    void Sort();
    void SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);

public:
    DirectX::SimpleMath::Vector3 Position;
    float Width;
    float Height;
    float Length;

    bool GravityAffected;

    const UINT MaxParticlesCount = 256 * 256;
    const UINT MaxParticlesInjectionCount = 100;
    UINT InjectionCount = 0;

    UINT ParticlesCount = MaxParticlesCount;

#pragma pack(push, 4)
    struct Particle
    {
        DirectX::SimpleMath::Vector4 Position;
        DirectX::SimpleMath::Vector4 Velocity;
        DirectX::SimpleMath::Vector4 Color0;
        DirectX::SimpleMath::Vector2 Size0Size1;
        float LifeTime;
        float MaxLifeTime = 1.0f;
    };
#pragma pack(pop)

#pragma pack(push, 4)
    struct ParticleDepths
    {
        UINT Index;
        float Depth;
    };
#pragma pack(pop)

#pragma pack(push, 4)
    struct ConstData
    {
        DirectX::SimpleMath::Matrix World;
        DirectX::SimpleMath::Matrix View;
        DirectX::SimpleMath::Matrix Proj;
        DirectX::SimpleMath::Vector4 DeltaTimeMaxParticlesGroupdimGround;
        DirectX::SimpleMath::Matrix InvView;
        DirectX::SimpleMath::Vector4 BoundingSphereInfo;
    };
#pragma pack(pop)

    struct CBSort
    {
        UINT iLevel;
        UINT iLevelMask;
        UINT iWidth;
        UINT iHeight;
    };

    struct DrawArgs
    {
        UINT IndexCountPerInstance;
        UINT InstanceCount;
        UINT StartIndexLocation;
        int BaseVertexLocation;
        UINT StartInstanceLocation;
    };

    struct EmitterParams
    {
        bool Enabled;
        float ParticlesPerSecond;
        DirectX::SimpleMath::Vector4 Color0;
        bool NudgeColorLum;
        bool NudgeColorHue;
        DirectX::SimpleMath::Vector3 Velocity;
        bool NudgeVelocity;
        float Size0;
        float Size1;
        float LifeTime;
        bool NudgeLifeTime;
        DirectX::SimpleMath::Vector3 Offset;
        DirectX::SimpleMath::Vector2 DynamicVelocityX;
        DirectX::SimpleMath::Vector2 DynamicVelocityY;
        DirectX::SimpleMath::Vector2 DynamicVelocityZ;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> bufArgs;

    enum class ComputeFlags
    {
        INJECTION = 1 << 0,
        SIMULATION = 1 << 1,
        ADD_GRAVITY = 1 << 2,
    };

    std::map<ComputeFlags, Microsoft::WRL::ComPtr<ID3D11ComputeShader>> ComputeShaders;

    Microsoft::WRL::ComPtr<ID3D11ComputeShader> SortShader;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> TransposeShader;

    ConstData constData;

    Particle* injectionParticles = new Particle[MaxParticlesInjectionCount];

    EmitterParams EmitterSettings = {};

    float GroundLevel;

    bool IsTextured = false;
    const wchar_t* TextureFileName;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShaderTex;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;

    ParticleSystem(Game* g);

    static void GetGroupSize(int particleCount, int& groupSizeX, int& groupSizeY);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void DestroyResources() override;
    void PrepareFrame() override {}

    void LoadShaders();
    void CreateBuffers();
    void AddParticle(const Particle& p);
    void SwapBuffers();
};
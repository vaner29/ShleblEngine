#pragma pack_matrix(row_major)

struct VS_IN
{
    float4 pos : POSITION0;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

float4 VSMain(VS_IN input) : SV_POSITION
{
    return mul(float4(input.pos.xyz, 1.0f), gWorldViewProj);
}

void PSMain()
{
} // No pixel shader output needed for depth-only pass
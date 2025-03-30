#pragma pack_matrix(row_major)

struct VS_IN
{
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 tex : TEXCOORD;
    float4 normal : NORMAL;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gInvTrWorld;
    float isSpinningFloor;
    float3 padding;
};

cbuffer cbPerScene : register(b1)
{
    float4 lightDir;
    float4 lightColorAmbStr;
    float4 viewDirSpecStr;
    float gTime;
    float3 padding2;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
#ifdef VERTEX_PASS_THROUGH
    output.pos = float4(input.pos.xyz, 1.0f);
#else
    output.pos = mul(float4(input.pos.xyz, 1.0f), gWorldViewProj);
#endif
    output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), gInvTrWorld);
    
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
#ifdef TREAT_TEX_AS_COL
    return input.tex;
#endif

    float4 objColor;
    if (isSpinningFloor > 0.5f)
    {
        // Extract tile and local coordinates
        float2 texCoord = input.tex.xy;
        float2 tileCoord = floor(texCoord);
        float2 localCoord = frac(texCoord);
        float2 centeredCoord = localCoord - 0.5f;

        // Determine rotation direction based on tile position (checkerboard pattern)
        float tileSum = tileCoord.x + tileCoord.y;
        bool isClockwise = fmod(tileSum, 2.0f) < 1.0f; // Even sum = clockwise, odd = counterclockwise

        // Rotation parameters
        float spinSpeed = 1.0f; // Radians per second
        float angle = gTime * spinSpeed;
        float cosA = cos(angle);
        float sinA = sin(angle);

        // Rotation matrix: clockwise or counterclockwise
        float2x2 rotationMatrix;
        if (isClockwise)
        {
            rotationMatrix = float2x2(cosA, sinA, -sinA, cosA); // Clockwise
        }
        else
        {
            rotationMatrix = float2x2(cosA, -sinA, sinA, cosA); // Counterclockwise
        }

        // Apply rotation and recombine
        float2 rotatedCoord = mul(centeredCoord, rotationMatrix);
        float2 finalCoord = tileCoord + (rotatedCoord + 0.5f);

        objColor = DiffuseMap.SampleLevel(Sampler, finalCoord, 0);
    }
    else
    {
        objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
    }

    float4 ambient = lightColorAmbStr.w * float4(lightColorAmbStr.xyz, 1.0f);
    float4 result = ambient * objColor;
    
    return float4(result.xyz, 1.0f);
}
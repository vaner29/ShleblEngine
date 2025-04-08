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
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL;
    float4 worldPos : TEXCOORD1;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gInvTrWorld;
    float4x4 gWorld;
    float isSpinningFloor;
    float3 diffuseColor;
    float3 specularColor;
    float shininess;
    struct
    {
        float4 position;
        float4 color;
    } pointLights[4];
    int numPointLights;
    float3 padding;
};

cbuffer cbPerScene : register(b1)
{
    float4 ambientStrength;
    float4 viewPos;
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
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), gWorld);
    
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
        float2 texCoord = input.tex.xy;
        float2 tileCoord = floor(texCoord);
        float2 localCoord = frac(texCoord);
        float2 centeredCoord = localCoord - 0.5f;

        float tileSum = tileCoord.x + tileCoord.y;
        bool isClockwise = fmod(tileSum, 2.0f) < 1.0f;

        float spinSpeed = 1.0f;
        float angle = gTime * spinSpeed;
        float cosA = cos(angle);
        float sinA = sin(angle);

        float2x2 rotationMatrix;
        if (isClockwise)
            rotationMatrix = float2x2(cosA, sinA, -sinA, cosA);
        else
            rotationMatrix = float2x2(cosA, -sinA, sinA, cosA);

        float2 rotatedCoord = mul(centeredCoord, rotationMatrix);
        float2 finalCoord = tileCoord + (rotatedCoord + 0.5f);

        objColor = DiffuseMap.SampleLevel(Sampler, finalCoord, 0);
    }
    else
    {
        objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
    }

    float3 N = normalize(input.normal.xyz);
    float3 V = normalize(viewPos.xyz - input.worldPos.xyz);
    float3 lighting = ambientStrength.w * float3(1.0f, 1.0f, 1.0f); // White ambient

    const float radius = 100.0f;
    for (int i = 0; i < numPointLights; i++)
    {
        float3 lightVec = pointLights[i].position.xyz - input.worldPos.xyz;
        float distance = length(lightVec);
        if (distance < radius)
        {
            float3 L = normalize(lightVec);
            float attenuation = 1.0f - (distance / radius);

            float diff = max(dot(N, L), 0.0f);
            float3 diffuse = diff * pointLights[i].color.xyz * diffuseColor * attenuation;

            float3 R = reflect(-L, N);
            float spec = pow(max(dot(R, V), 0.0f), shininess);
            float3 specular = spec * pointLights[i].color.xyz * specularColor * attenuation;

            lighting += diffuse + specular;
        }
    }

    float3 result = lighting * objColor.xyz;
    return float4(result, 1.0f);
}
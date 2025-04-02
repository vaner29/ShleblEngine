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
    float4 worldPos : TEXCOORD1; // Add world position for lighting
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gInvTrWorld;
    float isSpinningFloor;
    float3 diffuseColor; // Material diffuse color
    float3 specularColor; // Material specular color
    float shininess; // Specular shininess
    float padding; // Align to 16 bytes
};

cbuffer cbPerScene : register(b1)
{
    float4 lightDir; // Directional light direction
    float4 lightColor; // Light color (RGB) and intensity (A)
    float4 ambientStrength; // Ambient strength (xyz unused, w = strength)
    float4 viewPos; // Camera position in world space
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
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), gInvTrWorld); // Transform to world space
    
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

    // Normalize normal and light direction
    float3 N = normalize(input.normal.xyz);
    float3 L = normalize(-lightDir.xyz); // Negative because lightDir points toward light

    // Ambient
    float3 ambient = ambientStrength.w * lightColor.xyz;

    // Diffuse
    float diff = max(dot(N, L), 0.0f);
    float3 diffuse = diff * lightColor.xyz * diffuseColor;

    // Specular
    float3 V = normalize(viewPos.xyz - input.worldPos.xyz); // View direction
    float3 R = reflect(-L, N); // Reflection direction
    float spec = pow(max(dot(R, V), 0.0f), shininess);
    float3 specular = spec * lightColor.xyz * specularColor;

    // Combine lighting components
    float3 result = (ambient + diffuse + specular) * objColor.xyz;
    
    return float4(result, 1.0f);
}
#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

// Input/Output Structures
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
    float4 viewPos : VIEWPOS;
    float4 worldPos : TEXCOORD1;
    float4 lightSpacePos[CASCADE_COUNT] : TEXCOORD2; // For cascaded shadows
};

// Constant Buffers (matching your structures)
cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gInvTrWorld;
    float4x4 gWorld;
    float4x4 gWorldView; // Not used in VS but included for buffer compatibility
    float isSpinningFloor;
    float3 diffuseColor;
    float3 specularColor;
    float shininess;
    struct
    {
        float4 position;
        float4 color;
    } pointLights[10];
    int numPointLights;
    float3 padding;
};

cbuffer cbPerScene : register(b1)
{
    float4 lightPos; // Directional light direction
    float4 lightColor; // Directional light color
    float4 ambientSpecularPowType; // a - ambient, s - specular strength, p - power
    float4x4 gT; // Texture transform for shadows
    float gTime; // For spinning floor
    float3 padding2;
};

cbuffer cbCascade : register(b2)
{
    float4x4 gViewProj[CASCADE_COUNT]; // Cascade view-projection matrices
    float4 gDistances; // Cascade distances
};

// Textures and Samplers
Texture2D DiffuseMap : register(t0);
Texture2DArray CascadeShadowMap : register(t1);
SamplerState Sampler : register(s0);
SamplerComparisonState DepthSampler : register(s1);

// Vertex Shader
PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.pos = mul(float4(input.pos.xyz, 1.0f), gWorldViewProj);
    output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), gInvTrWorld);
    output.viewPos = mul(float4(input.pos.xyz, 1.0f), gWorldViewProj); // Approximate view space
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), gWorld);
    
    // Compute light space positions for each cascade
    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        output.lightSpacePos[i] = mul(float4(input.pos.xyz, 1.0f), gWorld);
        output.lightSpacePos[i] = mul(output.lightSpacePos[i], gViewProj[i]);
    }
    
    return output;
}

// Shadow Calculation (adapted from their shader)
float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN, PS_IN input)
{
    float depthValue = abs(posViewSpace.z);

    int layer = -1;
    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        if (depthValue < gDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = CASCADE_COUNT - 1; // Use the last cascade
    }

    // Fixed line: Use 'input' instead of 'output'
    float4 posLightSpace = input.lightSpacePos[layer];
    float3 projCoords = posLightSpace.xyz / posLightSpace.w;

    // Rest of the shadow calculation
    projCoords = mul(float4(projCoords, 1.0f), gT).xyz; // Texture transform
    float currentDepth = projCoords.z;

    if (currentDepth > 1.0f)
    {
        return 0.0f; // Outside shadow map
    }

    float bias = max(0.05f * (1.0f - dotN), 0.005f);
    const float biasModifier = 0.5f;
    if (layer == CASCADE_COUNT - 1)
    {
        bias *= 1.0f / (1000.0f * biasModifier);
    }
    else
    {
        bias *= 1.0f / (gDistances[layer] * biasModifier);
    }

    // Percentage-Closer Filtering (PCF)
    float shadow = 0.0f;
    float2 texelSize = 1.0f / 1024.0f; // Adjust based on shadow map size
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float2 offset = float2(x, y) * texelSize;
            shadow += CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy + offset, layer), currentDepth - bias);
        }
    }
    shadow /= 9.0f;

    return shadow;
}

// Pixel Shader
float4 PSMain(PS_IN input) : SV_Target
{
    // Spinning Floor Texture (from your shader)
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

    // Lighting Setup
    float3 N = normalize(input.normal.xyz);
    float3 V = normalize(-input.viewPos.xyz); // View direction (assuming viewPos is in view space)
    float3 lighting = ambientSpecularPowType.x * lightColor.xyz; // Ambient

    // Directional Light with Shadows (from their shader)
    float3 L_dir = normalize(-lightPos.xyz); // Assuming lightPos is direction
    float NdotL = max(dot(N, L_dir), 0.0f);
    float3 diffuse_dir = NdotL * lightColor.xyz * diffuseColor;
    float3 R_dir = reflect(-L_dir, N);
    float spec_dir = pow(max(dot(R_dir, V), 0.0f), ambientSpecularPowType.z);
    float3 specular_dir = ambientSpecularPowType.y * spec_dir * lightColor.xyz * specularColor;

    float shadow = ShadowCalculation(input.worldPos, input.viewPos, NdotL, input);
    lighting += (diffuse_dir + specular_dir) * (1.0f - shadow);

    // Point Lights (from your shader)
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
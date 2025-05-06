#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float2 tex : TEXCOORD;
};

cbuffer cbPerScene : register(b0)
{
	float4 lightPos;
	float4 lightColor;
    float ambientV;
    float specularV;
    float falloffV;
    float typeV;
	float4x4 gT;
	float4x4 gView;
    float4 SpotlightDirection; // Spotlight direction (normalized)
    float SpotlightConeAngle; // Cosine of the cone angle
    float SpotlightRange; // Maximum range of the spotlight
    float2 Padding; // Align to 16 bytes
};

cbuffer cbCascade : register(b1)
{
	float4x4 gViewProj[CASCADE_COUNT + 1];
	float4 gDistances;
};

Texture2D<float4> DiffuseTex : register(t0);
Texture2D<float3> WorldPositions : register(t1);
Texture2D<float3> Normals : register(t2);
Texture2DArray CascadeShadowMap : register(t3);
SamplerComparisonState DepthSampler : register(s0);

PS_IN VSMain(uint id: SV_VertexID)
{
	PS_IN output = (PS_IN)0;

	output.tex = float2(id & 1, (id & 2) >> 1);
	output.pos = float4(output.tex * float2(2, -2) + float2(-1, 1), 0, 1);
	
	return output;
}

float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN)
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
        layer = CASCADE_COUNT;
    }

    float4 posLightSpace = mul(float4(posWorldSpace.xyz, 1.0), gViewProj[layer]);
    float3 projCoords = posLightSpace.xyz / posLightSpace.w;

    projCoords = (mul(float4(projCoords, 1.0f), gT)).xyz;
    float currentDepth = projCoords.z;

    if (currentDepth > 1.0f)
    {
        return 0.0f;
    }


    // PCF
    float shadow = 0.0f;
    float2 texelSize = 1.0f / 2048.0f;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            shadow += CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy + float2(x, y) * texelSize, layer), currentDepth - 0.001f);
        }
    }
    shadow /= 9.0f;

    return shadow;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float3 norm = normalize(Normals.Load(int3(input.pos.xy, 0)));
    float4 worldPos = float4(WorldPositions.Load(int3(input.pos.xy, 0)).xyz, 1.0f);
    float4 viewPos = mul(worldPos, gView);
    float3 viewDir = normalize(-viewPos.xyz);
    float4 objColor = float4(DiffuseTex.Load(int3(input.pos.xy, 0)).xyz, 1.0f);

    float shadow = 0.0f;
    float attenuation = 1.0f;
    float3 lightDir = float3(0.0f, 0.0f, 0.0f);

    [branch]
    if (typeV == 0) // Directional light
    {
        lightDir = normalize(lightPos.xyz);
        shadow = ShadowCalculation(worldPos, viewPos, dot(norm, lightDir));
    }
    else if (typeV == 1) // Spotlight
    {
        float3 toLight = lightPos.xyz - worldPos.xyz;
        float distance = length(toLight);
        if (distance > SpotlightRange)
            return float4(0, 0, 0, 0); // Outside range

        lightDir = toLight / distance;
        float spotFactor = dot(lightDir, -SpotlightDirection.xyz);
        if (spotFactor < SpotlightConeAngle)
            return float4(0, 0, 0, 0); // Outside cone

        attenuation = 1.0f / (1.0f + distance * distance);
        // Optional: Add falloff based on spotFactor
        // attenuation *= pow(spotFactor, falloffV);
    }

    float4 ambient = ambientV * float4(lightColor.xyz, 1.0f) * objColor;
    float diff = max(dot(norm, lightDir), 0.0f) * attenuation;
    float4 diffuse = diff * float4(lightColor.xyz, 1.0f) * objColor;

    float3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), falloffV) * attenuation;
    float4 specular = specularV * spec * float4(lightColor.xyz, 1.0f);

    float4 result = ambient + (typeV == 0 ? shadow : 1.0f) * (diffuse + specular);
    return float4(result.xyz, 1.0f);
}
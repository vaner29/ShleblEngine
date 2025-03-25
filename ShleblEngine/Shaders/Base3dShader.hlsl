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
};

cbuffer cbPerScene : register(b1)
{
    float4 lightDir;
    float4 lightColorAmbStr;
    float4 viewDirSpecStr;
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

    float4 ambient = lightColorAmbStr.w * float4(lightColorAmbStr.xyz, 1.0f);
    float4 objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);

    float4 norm = normalize(input.normal);
    float diff = max(dot(norm, lightDir), 0.0f);
    float4 diffuse = diff * float4(lightColorAmbStr.xyz, 1.0f);

    float4 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirSpecStr.xyz, reflectDir.xyz), 0.0f), 32);
    float4 specular = viewDirSpecStr.w * spec * float4(lightColorAmbStr.xyz, 1.0f);

    float4 result = (ambient + diffuse + specular) * objColor;
	
    return float4(result.xyz, 1.0f);

	//return norm;
	//return DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
}
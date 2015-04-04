//=============================================================================
// Skybox.fx by Cody Carlson
//
// Does Skybox dome texturing! Based on Frank Luna's code
//=============================================================================

uniform extern float4x4 gWorldViewProj;

uniform extern texture gCubeMap;

sampler EnvMapS = sampler_state
{
	Texture = <gCubeMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

void VS(float3 posL : POSITION0, out float4 oPosH : POSITION0, out float3 oEnvTex: TEXCOORD0)
{
	oPosH = mul(float4(posL, 1.0f), gWorldViewProj).xyww;
	oEnvTex = posL;
}

float4 PS(float3 envTex : TEXCOORD0) : COLOR
{
	return texCUBE(EnvMapS, envTex);
}

technique SkyTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 VS();
		pixelShader = compile ps_2_0 PS();

		CullMode = None;
		ZFunc = Always;
		StencilEnable = true;
		StencilFunc = Always;

		StencilPass = Replace;
		StencilRef = 0;
	}
}
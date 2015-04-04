//=============================================================================
// Skybox.fx by Cody Carlson
//
// Does Skybox dome texturing! Based on Frank Luna's code
//=============================================================================

uniform extern float4x4 gWorldViewProj;

uniform extern texture gEnvMap;
static float gReflectivity = 1.0f;

float4 EnvMapPS(float3 normalW: TEXCOORD0, float3 toEyeW: TEXCOORD1, float2 tex0: TEXCOORD2) : COLOR
{
	normalW = normalize(normalW);
	toEyeW = normalize(toEyeW);

	float3 envMapTex = reflect(-toEyeW, normalW);
	float3 reflectedColor = texCUBE(EnvMapS, envMapTex);
}
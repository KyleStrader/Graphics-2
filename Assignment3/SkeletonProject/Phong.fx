//=============================================================================
// phong.fx by Cody Carlson
//
// Does phong lighting! Based on code here: http://www.gamasutra.com/view/feature/2866/implementing_lighting_models_with_.php?print=1
//=============================================================================

uniform extern texture gTex;

uniform extern float4x4 gWVP;
uniform extern float4x4 gWorldInverseTranspose;
uniform extern float4x4 gWorld;
uniform extern float3 gLightVecW;

uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;
uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float  gSpecularPower;

uniform extern float3 gEyePosW;

uniform extern texture gCubeMap;

uniform extern float gReflectivity;

sampler EnvMapS = sampler_state
{
	Texture = <gCubeMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_OUTPUT
{
	float4 posH: POSITION0;
	float3 normalW: TEXCOORD0;
	float3 posW: TEXCOORD1;
	float2 tex0: TEXCOORD2;
};

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

VS_OUTPUT PhongVS(float3 posL: POSITION0, float3 normalL : NORMAL0, float2 tex0 : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	output.normalW = normalize(output.normalW);

	output.posW = mul(float4(posL, 1.0f), gWorld).xyz;

	output.posH = mul(float4(posL, 1.0f), gWVP);

	output.tex0 = tex0;

	return output;
}

float4 PhongPS(float3 normalW : TEXCOORD0, float3 posW : TEXCOORD1, float2 tex0 : TEXCOORD2) : COLOR
{
	normalW = normalize(normalW);

	float3 texColor = tex2D(TexS, tex0).rgb;

	float3 lightVec = normalize(gLightVecW);

	float3 toEye = normalize(gEyePosW - posW);
	float3 r = reflect(-lightVec, normalW);
	float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(lightVec, normalW), 0.0f);

	float3 final;
	float3 ambient, diffuse, spec;

	/* Code for Environment Map reflections */
	float3 envMapTex = reflect(-toEye, normalW);
	float3 reflectedColor = texCUBE(EnvMapS, envMapTex);

	//float3 ambientMtrl = gReflectivity*reflectedColor + (1.0f - gReflectivity)*(gAmbientMtrl.rgb*texColor);
	//float3 diffuseMtrl = gReflectivity*reflectedColor + (1.0f - gReflectivity)*(gDiffuseMtrl.rgb*texColor);
	float ambientCoeff = .2f;
	float diffuseCoeff = .65f;
	float specCoeff = .15f;

	float3 ambientMtrl = (gAmbientMtrl.rgb*texColor*ambientCoeff);
	float3 diffuseMtrl = (gDiffuseMtrl.rgb*texColor*diffuseCoeff);
	float3 specMtrl = (gReflectivity*reflectedColor) + (1.0f - gReflectivity)*(gSpecularMtrl.rgb*texColor*specCoeff);

	//spec = t*(gSpecularMtrl.rgb*gSpecularLight.rgb).rgb;
	spec = t*(specMtrl.rgb*gSpecularLight.rgb).rgb;
	diffuse = s*(diffuseMtrl.rgb*gDiffuseLight.rgb);
	ambient = ambientMtrl*gAmbientLight.rgb;
	
	
	final = (ambient) + (diffuse) + (spec);
	return float4(final, gDiffuseMtrl.a);
}

technique PhongTech
{
	pass P0
	{
		// Specify the vertex and pixel shader associated with this pass.
		vertexShader = compile vs_2_0 PhongVS();
		pixelShader = compile ps_2_0 PhongPS();
	}
}
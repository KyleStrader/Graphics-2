//=============================================================================
// phong.fx by Cody Carlson
//
// Does phong lighting! Based on code here: http://www.gamasutra.com/view/feature/2866/implementing_lighting_models_with_.php?print=1
//=============================================================================

//Diffuse Map
uniform extern texture gTex;

//Normal Map
uniform extern texture gNormalMap;

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

//Cube Map
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

sampler NormalMapS = sampler_state
{
	Texture = <gNormalMap>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
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
	float3 toEyeT: TEXCOORD2;
	float3 lightDirT: TEXCOORD3;
	float2 tex0: TEXCOORD4;
};

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

VS_OUTPUT PhongVS(float3 posL: POSITION0,
	float3 tangentL : TANGENT0,
	float3 binormalL : BINORMAL0,
	float3 normalL : NORMAL0,
	float2 tex0 : TEXCOORD0)
{
	//Zero out our output
	VS_OUTPUT output = (VS_OUTPUT)0;

	//Build TBN-basis
	float3x3 TBN;
	TBN[0] = tangentL;
	TBN[1] = binormalL;
	TBN[2] = normalL;

	//Matrix transforms from object space to tangent space
	float3x3 toTangentSpace = transpose(TBN);

	//Transform eye position to local space
	float3 eyePosL = mul(float4(gEyePosW, 1.0f), gWorldInverseTranspose);

	//Transform to-eye vector to tangent space
	float3 toEyeL = eyePosL - posL;

	output.toEyeT = mul(toEyeL, toTangentSpace);

	//Transform light direction to tangent space
	float3 lightDirL = mul(float4(gLightVecW, 0.0f), gWorldInverseTranspose);
	output.lightDirT = mul(lightDirL, toTangentSpace);

	output.normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	output.normalW = normalize(output.normalW);

	//transform to homogenous clip space
	output.posW = mul(float4(posL, 1.0f), gWorld).xyz;
	output.posH = mul(float4(posL, 1.0f), gWVP);

	//Pass on texture coordinates to be interpolated in rasterization
	output.tex0 = tex0;

	return output;
}

float4 PhongPS(float3 normalW : TEXCOORD0, 
	float3 posW : TEXCOORD1, 
	float3 toEyeT: TEXCOORD2, 
	float3 lightDirT: TEXCOORD3, 
	float2 tex0 : TEXCOORD4) 
	: COLOR
{
	//Normal Mapping
	//Interpolated normals can become unnormal.
	toEyeT = normalize(toEyeT);
	lightDirT = normalize(lightDirT);

	//Light vector is opposite the direction of the light.
	float3 lightVecT = -lightDirT;

	//Sample normal map
	float3 normalT = tex2D(NormalMapS, tex0);

	//Expand from [0, 1] compressed interval to true [-1, 1] interval
	normalT = 2.0f*normalT - 1.0f;

	//make it a unit vector
	normalT = normalize(normalT);

	normalW = normalize(normalW);

	float3 texColor = tex2D(TexS, tex0).rgb;

	float3 lightVec = normalize(lightVecT/*gLightVecW*/);

	float3 toEye = normalize(gEyePosW - posW);
	float3 r = reflect(-lightVec, normalT/*NormalW*/);
	float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(lightVec, normalT/*NormalW*/), 0.0f);

	float3 final;
	float3 ambient, diffuse, spec;

	/* Code for Environment Map reflections */
	float3 envMapTex = reflect(-toEye, normalT/*NormalW*/);
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
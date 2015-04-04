
//=============================================================================
// Gouraud.fx by Kyle Strader.
//
// Does basic ambient, diffuse, and specular lighting on vertex level.
//=============================================================================

uniform extern texture gTex;

uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInverseTranspose;
uniform extern float4x4 gWVP;

uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;
uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float  gSpecularPower;
uniform extern float3 gLightVecW;
uniform extern float3 gEyePosW;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 posH: POSITION0;
	float4 diffuse : COLOR0;
	float4 spec : COLOR1;
	float2 tex0 : TEXCOORD0;
};

VS_OUTPUT GouraudLightingVS(float3 posL : POSITION0, float3 normalL : NORMAL0, float2 tex0: TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	normalW = normalize(normalW);

	float3 posW = mul(float4(posL, 1.0f), gWorld).xyz;

	output.posH = mul(float4(posL, 1.0f), gWVP);

	float3 lightVec = normalize(gLightVecW);

	float3 toEye = normalize(gEyePosW - posW);
	float3 r = reflect(-lightVec, normalW);
	float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(lightVec, normalW), 0.0f);

	float3 spec = t*(gSpecularMtrl*gSpecularLight).rgb;
	float3 diffuse = s*(gDiffuseLight).rgb;
	float3 ambient = gAmbientMtrl.rgb*gAmbientLight.rgb;

	output.diffuse.rgb = diffuse + ambient;
	output.diffuse.a = gDiffuseMtrl.a;
	output.spec = float4(spec, 0.0f);

	output.tex0 = tex0;

	return output;
}

float4 GouraudLightingPS(float4 c : COLOR0, float4 spec: COLOR1, float2 tex0 : TEXCOORD0) : COLOR
{
	float3 texColor = tex2D(TexS, tex0).rgb;

	float3 diffuse = c.rgb * texColor;

	return float4(diffuse + spec.rgb, c.a);
}

technique GouraudTech
{
	pass P0
	{
		// Specify the vertex and pixel shader associated with this pass.
		vertexShader = compile vs_2_0 GouraudLightingVS();
		pixelShader = compile ps_2_0 GouraudLightingPS();
	}
}

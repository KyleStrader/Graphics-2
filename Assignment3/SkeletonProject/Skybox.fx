//=============================================================================
// Skybox.fx by Cody Carlson
//
// Does Skybox dome texturing! Based on Frank Luna's code
//=============================================================================

float4x4 gWorldViewProj;

TextureCube gCubeMap;

sampler TexS = sampler_state
{
	Texture = <gCubeMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VertexIn
{
	float3 PosL: POSITION;
};

struct VertexOut
{
	float4 PosH: POSITION0;
	float3 PosL: TEXCOORD0;
	float2 tex0: TEXCOORD1;
};

VertexOut VS(VertexIn vin, float2 tex0:TEXCOORD1)
{
	VertexOut vout;

	// Set z = w so that z/w = 1
	// means that our skybox will always be the last thing drawn
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	// Use local vertex position as cubemap lookup vector
	vout.PosL = vin.PosL;
	vout.tex0 = tex0;

	return vout;
}

float4 PS(VertexOut pin) : COLOR
{
	float3 rgbColor = tex2D(TexS, pin.tex0).rgb;
	float4 color = float4(rgbColor.rgb, 1);
	return color;
}

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	DepthFunc = LESS_EQUAL;
};

technique SkyTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 VS();
		pixelShader = compile ps_2_0 PS();

		//SetRasterizerState(NoCull);
		//SetDepthStencilState(LessEqualDSS, 0);
	}
}
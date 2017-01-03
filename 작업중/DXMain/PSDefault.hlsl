#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

// (������) �� �����Ϳ� ���� ��� �Լ��Դϴ�.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	float4 cColor = gMaterialColor;

	float Depth = input.position.z / input.position.w;

	return (PackGBuffer(cColor.xyz, normalize(input.normal), input.positionW, gSpecIntensity, gSpecExp, Depth));

}
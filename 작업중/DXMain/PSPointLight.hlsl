#include "UnpackGBuffer.hlsli"
#include "CalcPointLight.hlsli"



struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	uint2 UV		: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};


float4 main(DS_OUTPUT input) : SV_TARGET
{

	//return float4(1.0,1.0,1.0,1.0);

	//GBuffer ����ŷ
	float2 uv = input.Position.xy;
	SURFACE_DATA gbd = UnpackGBuffer(uv);

	//���� ����ü�� ������ ��ȯ
	Material mat;
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;
	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y + gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//���� ���� ���
	float4 finalColor;
	finalColor.xyz = CalcPoint(gbd.PositionW, mat, input.instanceID);
	//finalColor.xyz = mat.diffuseColor.xyz;
	finalColor.w = 1.0f;

	return finalColor;
}
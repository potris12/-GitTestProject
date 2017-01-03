
//#include "Directional_AmbientLight.hlsli"
#include "UnpackGBuffer.hlsli"
#include "CalcDirectionalLight.hlsli"

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{


	SURFACE_DATA gbd = UnpackGBuffer(input.position.xy);

	//SURFACE_DATA gbd = UnpackGBuffer(input.texCoord);

	//�����͸� ���� ����ü�� ��ȯ
Material mat;
mat.normal = gbd.Normal.xyz;
mat.diffuseColor.xyz = gbd.Color;
mat.diffuseColor.w = 1.0f;//���� ������

mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
mat.specIntensity = gbd.SpecInt;

//���� ��ġ ����
float3 positionW = CalcWorldPos(input.position.xy, gbd.LinearDepth);

//�����Ʈ �� �𷺼ų� ����Ʈ ���� ���
float4 finalColor;
//return float4(gbd.Normal.xyz, 1.0);


finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz);
finalColor.xyz += CalcDirectional(gbd.PositionW, mat);
//finalColor.xyz = mat.normal;
finalColor.w = 1.0f;

return finalColor;
}
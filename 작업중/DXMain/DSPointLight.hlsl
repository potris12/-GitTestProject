#define MAX_POINT_LIGHT 800

struct PointLight {
	float4x4 LightProjection;
};

cbuffer cbPointLightDomain : register(b0) {
	PointLight gPointLight[MAX_POINT_LIGHT];
}


struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	uint2 UV		: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

// �������� ����մϴ�.
struct HS_CONTROL_POINT_OUTPUT
{
	float4 HemiDir : POSITION; //position + instanceID
	uint instanceID : TEXCOORD0;
};

// ��ġ ��� �����͸� ����մϴ�.
struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4]			: SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
	float Inside[2]			: SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

//�׼����̼��� ������ �� ������ ���� ��ȯ�� ó���ؾ��� �� ����!!
[domain("quad")]
DS_OUTPUT main( HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, 1> quad){


	//Ŭ�� ��ǥ ����
	float2 posClipSpace = UV.xy * 2.0 - 1.0;

	//�߽ɿ��� ���� �� ������ ���� �Ÿ� ���
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);

	//Ŭ�� ���������� ���� ��ġ ����
	float3 normDir = normalize(float3(posClipSpace.xy, maxLen - 1.0) * quad[0].HemiDir.xyz);
	float4 posLS = float4(normDir.xyz, 1.0);

	//�������� ��ȿ� ���� ��� ��ȯ�� ��
	//UV ��ǥ ����
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gPointLight[quad[0].HemiDir.w].LightProjection);
	//Output.Position = posLS;

	//Ŭ�� ���� ��ġ ����
	Output.UV = Output.Position.xy / Output.Position.w;

	//instanceID get!!
	Output.instanceID = quad[0].HemiDir.w;
	return Output;

}

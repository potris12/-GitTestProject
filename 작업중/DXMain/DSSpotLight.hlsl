#define MAX_SPOT_LIGHT 800

//spot
#define CylinderPortion 0.2
#define ExpendAmount (1.0 + CylinderPortion)

struct SpotLight {
	float4x4 LightProjection;

	float SpotCosOuterCone;
	float SpotSinOuterCone;
	float2 tempData;//float4�� ���߱� ���� ������
};

cbuffer cbSpotLightDomain : register(b0) {
	SpotLight  gSpotLight[MAX_SPOT_LIGHT];
}
//spot

struct DS_OUTPUT
{
	float4 Position  : SV_POSITION;
	uint2 UV : TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

// �������� ����մϴ�.
struct HS_CONTROL_POINT_OUTPUT
{
	float4 CapsuleDir : POSITION; //position + instanceID
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
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, 4> quad) {

	uint instanceID = quad[0].instanceID;
	//Ŭ�� ��ǥ ����
	float2 posClipSpace = UV.xy * float2(2.0, -2.0) + float2(-1.0, 1.0);

	//uv ���� ����� �ΰ� ���ؽ� ������ ���
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);
	
	//���� ���ؽ��� �޽� ������ ���� ��ȯ
	float2 posClipSpaceNoCylAbs = saturate(posClipSpaceAbs * ExpendAmount);
	float maxLenNoCapsule = max(posClipSpaceNoCylAbs.x, posClipSpaceNoCylAbs.y);
	float2 posClipSpaceNoCyl = sign(posClipSpace.xy) * posClipSpaceNoCylAbs;
	
	//�ݱ� ��ġ�� �����ڸ��� ���� ���ؽ��� ��ȯ
	float3 halfSpherePos = normalize(float3(posClipSpaceNoCyl.xy, 1.0 - maxLenNoCapsule));

	//���� ���� �ظ� ũ��� ������ ����
	halfSpherePos = normalize(float3(halfSpherePos.xy * gSpotLight[instanceID].SpotSinOuterCone, gSpotLight[instanceID].SpotCosOuterCone));

	//���� ���ؽ��� ������ ��� ( ���� �ظ��� 0 )
	float cylinderOffsetZ = saturate((maxLen * ExpendAmount - 1.0) / CylinderPortion);

	//���� ���ؽ��� ���� ��ġ�� ������
	float4 posLS = float4(halfSpherePos.xy * (1.0 - cylinderOffsetZ), halfSpherePos.z - cylinderOffsetZ * gSpotLight[instanceID].SpotCosOuterCone, 1.0);


	//���� ������ ���� ��� ��ȯ�� ��
	//UV ��ǥ ����
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gSpotLight[instanceID].LightProjection);
	Output.UV = Output.Position.xy / Output.Position.w;
	//Output.UV = Output.UV * float2(0.5, -0.5) + 0.5;
	Output.instanceID = instanceID;

	return Output;
}

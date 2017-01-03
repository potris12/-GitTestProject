#define MAX_CAPSULE_LIGHT 800

//capsule
#define CylinderPortion 0.5
#define SpherePortion (1.0 - CylinderPortion)
#define ExpendAmount (1.0 + CylinderPortion)

struct CapsuleLight {
	float4x4 LightProjection;
	
	float CapsuleLightRange;
	float HalfSegmentLen;
	float2 CapsuleTempData;//������ ������ float4 ���߷��� ���
};

cbuffer cbCapsuleLightDomain : register(b0) {
	CapsuleLight gCapsuleLight[MAX_CAPSULE_LIGHT];
}
//capsule

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

	float2 posClipSpaceNoCylAbs = saturate(posClipSpaceAbs * ExpendAmount);
	float maxLenNoCapsule = max(posClipSpaceNoCylAbs.x, posClipSpaceNoCylAbs.y);
	float2 posClipSpaceNoCyl = sign(posClipSpace.xy) * posClipSpaceNoCylAbs;

	float3 halfSpherePos = normalize(float3(posClipSpaceNoCyl.xy, 1.0 - maxLenNoCapsule));

	//Ŭ�� ������ ������ġ ����
	//float cylinderOffsetZ = saturate(maxLen - min(maxLenNoCapsule, SpherePortion)) / CylinderPortion;
	float cylinderOffsetZ = saturate((maxLen * ExpendAmount - 1.0f) / CylinderPortion);
	
	halfSpherePos *= gCapsuleLight[instanceID].CapsuleLightRange;

	float4 posLS = float4(halfSpherePos.xy, halfSpherePos.z 
		+ gCapsuleLight[instanceID].HalfSegmentLen 
		- cylinderOffsetZ * gCapsuleLight[instanceID].HalfSegmentLen, 1.0f);

	//float4 posLS = float4(halfSpherePos.xy, halfSpherePos.z
	//	+ 50
	//	- cylinderOffsetZ * 50, 1.0f);

	//���ؽ��� ������ ĸ�� ��ġ�� �̵�
	posLS *= quad[0].CapsuleDir;

	//���� ������ ���� ��� ��ȯ�� ��
	//UV ��ǥ ����
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gCapsuleLight[instanceID].LightProjection);
	Output.UV = Output.Position.xy / Output.Position.w;
	//Output.UV = Output.UV * float2(0.5, -0.5) + 0.5;
	Output.instanceID = instanceID;

	return Output;
}

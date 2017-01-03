static const float3 HemilDir[2] = {
	float3(1.0f, 1.0f, 1.0f),
	float3(-1.0f, 1.0f, -1.0f)
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	uint instanceID : TEXCOORD0;
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
	float Edges[4] : SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
	float Inside[2] : SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

// ��ġ ��� �Լ�
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants()
{
	HS_CONSTANT_DATA_OUTPUT Output;

	float tessFactor = 18.0f;

	// ���⿡ ����� ����� �ڵ带 �����մϴ�.
	//�󸶳� ���� ���ΰ�.
	Output.Edges[0] =
		Output.Edges[1] =
		Output.Edges[2] =
		Output.Edges[3] = tessFactor;

	Output.Inside[0] = Output.Inside[1] = tessFactor; 
		// ���� ��� ��� ���� ���� ����(tessellation) �μ��� ����� �� �ֽ��ϴ�.

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(1)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(InputPatch<VS_OUTPUT, 1> input, uint PatchID : SV_PrimitiveID)
{
	HS_CONTROL_POINT_OUTPUT Output;

	uint instanceID = input[0].instanceID;
	// ���⿡ ����� ����� �ڵ带 �����մϴ�.
	Output.HemiDir = float4(HemilDir[PatchID], instanceID);
	Output.instanceID = instanceID;
	return Output;
}

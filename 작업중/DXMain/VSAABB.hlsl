////struct VS_INPUT {
////	float3 positionW : INSTANCEPOS;
////	float3 extend  : EXTEND;
////};
//
//struct VS_OUTPUT {
//	uint instance : TEXCOORD;
//	//float3 extend : TEXCOORD0;
//};
//
//VS_OUTPUT main( uint instance : SV_InstanceID)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//	//���� ��ȯ�� �ʿ����. �׳� ������
//	output.instance = instance;
//
//	return output;
//}
struct VS_INS_INPUT{
	//float3 size : SIZE;
	float3 position : INSTANCEPOS;
	float3 extend : TEXCOORD;
};

struct VS_INS_OUTPUT{
	//float3 size : SIZE;
	float3 position : POSITION;
	float3 extend : EXTEND;
};

VS_INS_OUTPUT main(VS_INS_INPUT input)
{
	//���� ��ǥ
	//float4 position = float4(0, 0, 0, 1.0f);


	VS_INS_OUTPUT output = (VS_INS_OUTPUT)0;
	//output.size = input.size;
	output.position = input.position;
	output.extend = input.extend;
	//output.positionW = position.xyz;


	//�ԷµǴ� ������ size�� �״�� ����Ѵ�. 
	//output.color = input.color;
	//�ԷµǴ� ������ ������ �״�� ����Ѵ�. 
	return output;
}


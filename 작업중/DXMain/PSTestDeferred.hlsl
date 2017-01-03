

SamplerState        gssSamplerState		: register(s0);

Texture2D           gtxtColorSpecInt	: register(t0);
//Texture2D           gtxtNormal_Depth	: register(t1);
//Texture2D           gtxtSpecPow	 		: register(t2);

//static const float2 g_SpecPowerRange = { 0.1, 250.0 };
//
//cbuffer cbgbufferUnpack : register(b0) {
//	float4 PerspectiveValues	: packoffset(c0);
//	float4x4 ViewInv			: packoffset(c1);
//}

//float ConvertDepthToLinear(float depth) {
//	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
//	return linearDepth;
//}


struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


struct SURFACE_DATA {
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecInt;
	float SpecPow;
};


//
//SURFACE_DATA UnpackGBuffer(float2 texCoord) {
//	SURFACE_DATA Out = (SURFACE_DATA)0;
//
//	//���̽� ����� ����ŧ�� ���� �� ����
//	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
//	Out.Color = baseColorSpecInt.xyz;
//	Out.SpecInt = baseColorSpecInt.w;
//
//	//��� ���ø� �� ��ü ���� ��ȯ �� ����ȭ
//	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
//	Out.Normal = normal_depth.xyz;
//	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
//
//	//���� �� ���� �� ���� ���� ������ ��ȯ
//	float depth = normal_depth.w;
//	Out.LinearDepth = ConvertDepthToLinear(depth);
//
//	//���� ���� ���� ���� ����ŧ�� �Ŀ� ������ ����
//	float SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
//	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;
//
//	return Out;
//}
/*
Texture2D           gtxtColorSpecInt	: register(t0);
Texture2D           gtxtNormal			: register(t1);
Texture2D           gtxtSpecPow	 		: register(t2);
Texture2D           gtxtDepth	 		: register(t3);
*/

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	//SURFACE_DATA gbd = UnpackGBuffer(input.texCoord);


	//------------------------------------- ���� -----------------------------------------------
	float4 cColor = gtxtColorSpecInt.Sample(gssSamplerState, input.texCoord);
	return float4(cColor.xyz, 1.0);
//	return(float4(gbd.Normal, 1.0f));

}
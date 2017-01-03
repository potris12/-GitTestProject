

//specpowr ����ȭ 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };


struct PS_GBUFFER_OUT {
	float4 ColorSpecInt : SV_TARGET0;
	float4 Normal_Depth : SV_TARGET1;
	float4 PositionW_SpecPow : SV_TARGET2;	
};

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float3 PositionW, float SpecIntensity, float SpecPower, float Depth) {
	PS_GBUFFER_OUT Out = (PS_GBUFFER_OUT)0;

	//����ŧ�� �Ŀ� ����ȭ
	float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

	//GBuffer ����ü�� ������ ��ŷ
	Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
	Out.Normal_Depth = float4(Normal.xyz * 0.5 + 0.5, Depth);
	//Out.Normal_Depth = float4(Normal.xyz, Depth);

	//positionW/ specPow�� ��ŷ
	Out.PositionW_SpecPow.w = SpecPowerNorm;
	Out.PositionW_SpecPow.xyz = PositionW;

	return Out;
}
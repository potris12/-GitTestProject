cbuffer DownScaleConstants : register(b0) {
	//�ٿ� ������ �ػ� ��� x �ʺ�, y ����
	uint2 Res : packoffset(c0);
	//�ٿ���� �̹����� �� �ȼ� ��
	uint Domain : packoffset(c0.z);
	//ù��° pass�� ����� �׷� ��
	uint GroupSize : packoffset(c0.w);
	//������
	float Adaptation : packoffset(c1);
	//bloom �Ӱ谪 ����
	float fBloomThreshold : packoffset(c1.y);
}

Texture2D<float4> HDRDownScaleTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);
RWTexture2D<float4> Bloom : register(u0);

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

#define groupthreads 128

[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint2 CurPixel = uint2(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x);

	//�ȼ� ���� ����.. ? ???/ later ����
	if (CurPixel.y < Res.y) {
		float4 color = HDRDownScaleTex.Load(int3(CurPixel, 0));
		float Lum = dot(color, LUM_FACTOR);
		float avgLum = AvgLum[0];

		//Find the color scale
		float colorScale = saturate(Lum - avgLum * fBloomThreshold);

		Bloom[CurPixel.xy] = color * colorScale;
		//Bloom[CurPixel.xy] = float4(AvgLum[0], AvgLum[0], AvgLum[0], 1.0);
	}
}


//ambient
cbuffer CameraPosition : register(b1) {
	float4x4 CameraProjectionMtx:packoffset(c0);
	float4 EyePosition:packoffset(c4);
}
//ambient
//directional
cbuffer Dir_AmbLightConstants : register(b2) {
	float4 DirToLight:packoffset(c0);
	float4 DirLightColor:packoffset(c1);
	float4 DirLightPower:packoffset(c2);

	float4 AmbientDown : packoffset(c3);
	float4 AmbientRange : packoffset(c4);
	float4 AmbientUp : packoffset(c5);
	float4 AmbientColor : packoffset(c6);
}
cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

//directional
float3 CalcDirectional(float3 position, Material material) {
	//�� ��ǻ��
	float4 dirToLight = normalize(DirToLight);
	float NDotL = dot(dirToLight.xyz, material.normal);
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);

	//�� ����ŧ��
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + DirToLight.xyz);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}
//directional
//ambient
float3 CalcAmbient(float3 normal, float3 color) {
	//[-1, 1]���� [0, 1]���� ��ȯ
	float up = normal.y * 0.5 + 0.5;

	//ambient�� ���
	float3 Ambient = AmbientDown.xyz + up * AmbientUp.xyz;

	//���� ���� �����Ʈ �� ����
	return Ambient * color;
}
//ambient

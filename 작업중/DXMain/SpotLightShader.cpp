#include "stdafx.h"
#include "SpotLightShader.h"

bool CSpotLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSSpotLight.cso"));
	{
		m_pd3dDevice->CreateVertexShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &m_vertexShader
		);

	}

	if (m_vertexShader) return true;

	return false;


}
bool CSpotLightShader::CreateHS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("HSSpotLight.cso"));
	{
		m_pd3dDevice->CreateHullShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_hullShader
		);

	}

	if (m_hullShader) return true;

	return false;
}
bool CSpotLightShader::CreateDS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("DSSpotLight.cso"));
	{
		m_pd3dDevice->CreateDomainShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_domainShader
		);

	}

	if (m_domainShader) return true;

	return false;
}
bool CSpotLightShader::CreatePS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("PSSpotLight.cso"));
	{
		m_pd3dDevice->CreatePixelShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_pixelShader
		);

	}

	if (m_pixelShader) return true;

	return false;
}
//create shader


CSpotLightShader::CSpotLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CSpotLightShader::~CSpotLightShader() {

}
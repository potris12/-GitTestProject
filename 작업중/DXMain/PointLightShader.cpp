#include "stdafx.h"
#include "PointLightShader.h"

bool CPointLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSPointLight.cso"));
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
bool CPointLightShader::CreateHS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("HSPointLight.cso"));
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
bool CPointLightShader::CreateDS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ����m�� �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("DSPointLight.cso"));
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
bool CPointLightShader::CreatePS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("PSPointLight.cso"));
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


CPointLightShader::CPointLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CPointLightShader::~CPointLightShader() {

}
#include "stdafx.h"
#include "CapsuleLightShader.h"

bool CCapsuleLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSCapsuleLight.cso"));
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
bool CCapsuleLightShader::CreateHS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("HSCapsuleLight.cso"));
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
bool CCapsuleLightShader::CreateDS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("DSCapsuleLight.cso"));
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
bool CCapsuleLightShader::CreatePS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("PSCapsuleLight.cso"));
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


CCapsuleLightShader::CCapsuleLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CCapsuleLightShader::~CCapsuleLightShader() {

}
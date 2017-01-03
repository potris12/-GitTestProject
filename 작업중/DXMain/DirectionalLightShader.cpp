#include "stdafx.h"
#include "DirectionalLightShader.h"

bool CDirectionalLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSDirectionalLight.cso"));
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
bool CDirectionalLightShader::CreatePS() {

	// ������ hlsl�� cso �ε� �� PS ���� : ���� ��Ƽ������� ������ �ؾ� �� �κ�
	auto loadPS = ReadCSOFile(TEXT("PSDirectionalLight.cso"));
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


CDirectionalLightShader::CDirectionalLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CDirectionalLightShader::~CDirectionalLightShader() {

}
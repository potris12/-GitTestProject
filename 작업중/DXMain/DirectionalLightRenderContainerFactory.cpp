#include "stdafx.h"
#include "DirectionalLightRenderContainerFactory.h"

void CDirectionalLightRenderContainerFactory::CreateContainer() {
	m_pContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
};

void CDirectionalLightRenderContainerFactory::CreateMesh() {

	//--------------------------------�޽� ����------------------------
	shared_ptr<CMesh> pMesh = make_shared<CMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	m_pContainer->SetMesh(pMesh);//mesh set�� ���ÿ� instancing buffer ���� �� set
								 //--------------------------------�޽� ����------------------------


};
void CDirectionalLightRenderContainerFactory::CreateSharedBuffer(int nMaxObjects) {
	m_pContainer->CreateSharedBuffer(nMaxObjects, sizeof(DIRECTIONAL_LIGHT));
}
void CDirectionalLightRenderContainerFactory::CreateShader() {
	//--------------------------------���̴� ����-----------------------
	shared_ptr<CShader> pShader = make_shared<CDirectionalLightShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->Begin();
	m_pContainer->SetShader(pShader);
	//--------------------------------���̴� ����-----------------------

};
void CDirectionalLightRenderContainerFactory::CreateTexture() {

};
void CDirectionalLightRenderContainerFactory::CreateMaterial() {
	//-------------------------------���� ����-----------------------------
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	pMaterial->Begin();
	pMaterial->SetMaterialInfo(XMFLOAT4(0.5, 0.5, 0.5, 1.0), 4, 1);
	m_pContainer->SetMaterial(pMaterial);
	//-------------------------------���� ����-----------------------------
}




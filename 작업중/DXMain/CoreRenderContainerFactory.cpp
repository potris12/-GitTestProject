#include "stdafx.h"
#include "CoreRenderContainerFactory.h"

void CCoreRenderContainerFactory::CreateContainer() {
	m_pContainer = new CCoreRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
};

void CCoreRenderContainerFactory::CreateMesh(int nMaxObject) {

};
void CCoreRenderContainerFactory::CreateShader() {
	//--------------------------------���̴� ����-----------------------
	shared_ptr<CShader> pShader = make_shared<CInstancingShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->Begin();
	m_pContainer->SetShader(pShader);
	//--------------------------------���̴� ����-----------------------
};
void CCoreRenderContainerFactory::CreateTexture() {
	
};
void CCoreRenderContainerFactory::CreateMaterial() {
	
};
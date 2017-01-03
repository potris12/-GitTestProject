#include "stdafx.h"
#include "PlaneRenderContainerFactory.h"

void CPlaneRenderContainerFactory::CreateContainer() {

	m_pContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	
};

void CPlaneRenderContainerFactory::CreateMesh() {
	//--------------------------------�޽� ����------------------------
	shared_ptr<CMesh> pMesh = make_shared<CPlaneMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	m_pContainer->SetMesh(pMesh);//mesh set�� ���ÿ� instancing buffer ���� �� set
	//--------------------------------�޽� ����------------------------

};
void CPlaneRenderContainerFactory::CreateSharedBuffer(int nMaxObjects) {
	m_pContainer->CreateSharedBuffer(nMaxObjects, sizeof(VS_VB_INSTANCE));
}
void CPlaneRenderContainerFactory::CreateShader() {
	//--------------------------------���̴� ����-----------------------
	shared_ptr<CShader> pShader = make_shared<CTestShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->Begin();
	m_pContainer->SetShader(pShader);
	//--------------------------------���̴� ����-----------------------

};
void CPlaneRenderContainerFactory::CreateTexture() {
	
};
void CPlaneRenderContainerFactory::CreateMaterial() {
	//-------------------------------���� ����-----------------------------
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	pMaterial->Begin();
	pMaterial->SetMaterialInfo(XMFLOAT4(0.0, 1.0, 0.0, 1.0), 8, 2);
	m_pContainer->SetMaterial(pMaterial);
	//-------------------------------���� ����-----------------------------	
};
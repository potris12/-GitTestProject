#include "stdafx.h"
#include "Light.h"

bool CLight::Begin() { 

	return CGameObject::Begin();
};

//instance buffer controll base
void CLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {
	//����ȯ
	VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)ppMappedResources[0];
	//transpose ���� ���� ����
	pnInstances[nInstance].m_xmmtxWorld = XMMatrixTranspose(GetWorldMtx());

}




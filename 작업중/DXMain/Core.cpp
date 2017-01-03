#include "stdafx.h"
#include "Core.h"
#include "RenderContainer.h"

//----------------------------dxobject-----------------------------
bool CCore::Begin() {

	//object_id set
	m_objectID = object_id::OBJECT_CORE;
	//m_partID = part_id::PART_CORE;

	//hp set
	m_fHP = 100;
	m_fSize = 10;

	//slot Init
	m_ppPart = new CPart*[6];
	for (int i = 0; i < 6; ++i) {
		m_ppPart[i] = nullptr;
	}
	m_pfRotationVelocity = new float[6];
	for (int i = 0; i < 6; ++i) {
		m_pfRotationVelocity[i] = 0.0f;
	}

	//return CPart::Begin();
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 

	//------------------------------test---------------------------
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//SetComponent(pComponent);
	//------------------------------test---------------------------

	return CPart::Begin();
}
bool CCore::End() {
	for (int i = 0; i < 6; ++i) {
		if (m_ppPart[i]) delete m_ppPart[i];
	}
	if (m_ppPart) delete[] m_ppPart;

	if (m_pfRotationVelocity) delete[] m_pfRotationVelocity;

	return CPart::End();
}
//----------------------------dxobject-----------------------------


void CCore::GetPartsList(list<CPart*>& lParts) {
	
	lParts.push_back(this);

	for (int i = 0; i < 6; ++i) {
		//�� slot�� parts�� �ִٸ� list�� ������ ����!
		if (m_ppPart[i]) m_ppPart[i]->GetPartsList(lParts);
	}

}
void CCore::RegistToContainer(){
	m_pRenderContainer->AddObject(this);

	for (int i = 0; i < 6; ++i) {
		//�� slot�� parts�� �ִٸ� Animate!
		if (m_ppPart[i]) m_ppPart[i]->RegistToContainer();
	}
}
void CCore::Animate(float fTimeElapsed) {
	
	//core�� Animate!
	CGameObject::Animate(fTimeElapsed);

	//--------------------test----------------------
	XMVECTOR xmvDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	xmvDir += -GetRight() * m_pfRotationVelocity[0];
	xmvDir += -GetUp() * m_pfRotationVelocity[1];
	xmvDir += -GetLook() * m_pfRotationVelocity[2];
	xmvDir += GetRight() * m_pfRotationVelocity[3];
	xmvDir += GetUp() * m_pfRotationVelocity[4];
	xmvDir += GetLook() * m_pfRotationVelocity[5];

	Move(xmvDir, 10*fTimeElapsed);
	//--------------------test----------------------

	//part�� Animate! �� ���� �ٸ��� .���� ���� Animate �� ����
	XMMATRIX xmmtxParent = XMLoadFloat4x4(&m_xmf4x4World);
	for (int i = 0; i < 6; ++i) {
		//�� slot�� parts�� �ִٸ� Animate!
		if (m_ppPart[i]) m_ppPart[i]->Animate(fTimeElapsed, xmmtxParent);
	}

}

bool CCore::SetEquip(int slot, CPart* pPart) {

	if (nullptr == m_ppPart[slot]) {
		//��� set 
		if (false == pPart->Equiped(slot, this))return false;
		m_ppPart[slot] = pPart;

		return true;
	}

	return false;

}

void CCore::ChangePartState(int slot, part_state partState, CPart* pPart) {

	switch (partState) {
	case part_state::STATE_NEW:
		NewPart(slot, pPart);
		break;
	case part_state::STATE_DELETE:
		break;
	}
	
}

void CCore::NewPart(int slot, CPart* pPart) {
	object_id id = pPart->GetObjectID();
	float fVelocity = 0;

	switch (id) {
	case object_id::OBJECT_WEAPON:
		break;

	case object_id::OBJECT_BOOST:
		fVelocity = (pPart->GetAbilityValue()).m_fVelocity;
		m_pfRotationVelocity[slot] += fVelocity;
		break;

	case object_id::OBJECT_ADAPTOR:
		
		break;
	}

}

CCore::CCore() : CPart(){

};
CCore::~CCore() {

};
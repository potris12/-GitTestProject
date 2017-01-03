#include "stdafx.h"
#include "AnimateComponent.h"


bool CAnimateComponent::Begin() {

	return TRUE;
};

bool CAnimateComponent::End() {

	//�ڽ��� ������
	m_pOwner = NULL;

	return CComponent::End();
};

void CAnimateComponent::Update(float fTimeElapsed) {

	//1. owner mtx get
	//2. animate
	//3. owner mtx set
	m_pOwner->SetWorldMtx(Animate(m_pOwner->GetWorldMtx(), fTimeElapsed));
}

CAnimateComponent::CAnimateComponent() : CComponent(){
	//������Ʈ id
	m_ID = COMPONENTID_END;

	//���� ������ ������Ʈ�� ��Ƴ��� �йи� id
	//ex) geometry ����� render�� �׳� render
	m_FamilyID = ANIMATE;

	//�ڽ��� ������
	m_pOwner = NULL;
}

CAnimateComponent::~CAnimateComponent() {

}
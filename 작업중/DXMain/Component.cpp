#include "stdafx.h"
#include "Component.h"

bool CComponent::Begin() {

	return TRUE;
};

bool CComponent::End() {
	//������Ʈ id
	m_ID = COMPONENTID_END;

	//���� ������ ������Ʈ�� ��Ƴ��� �йи� id
	//ex) geometry ����� render�� �׳� render
	m_FamilyID = FAMILYID_END;

	return TRUE;
};

void CComponent::SetOwner(CGameObject* pObject) {
	if (!pObject)return;

	//������ ������ ���������� �Ű澲�� �ʴ´�.
	m_pOwner = pObject;
}

CComponent::CComponent() {
	//������Ʈ id
	m_ID = COMPONENTID_END;

	//���� ������ ������Ʈ�� ��Ƴ��� �йи� id
	//ex) geometry ����� render�� �׳� render
	m_FamilyID = FAMILYID_END;

	//�ڽ��� ������
	m_pOwner = NULL;
}
CComponent::~CComponent() {

}
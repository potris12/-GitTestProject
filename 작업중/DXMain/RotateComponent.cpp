#include "stdafx.h"
#include "RotateComponent.h"


bool CRotateComponent::Begin() {
	m_fRotate = static_cast<float>(rand() % 180);

	return TRUE;
};

bool CRotateComponent::End() {

	//�ڽ��� ������
	m_pOwner = NULL;

	return CComponent::End();
};
XMMATRIX CRotateComponent::Animate(XMMATRIX xmmtxWorld, float fTimeElapsed) {

	float fRadian = XMConvertToRadians( m_fRotate * fTimeElapsed);

	return XMMatrixMultiply(XMMatrixRotationY(fRadian), xmmtxWorld);
}

CRotateComponent::CRotateComponent() : CAnimateComponent() {
	//������Ʈ id
	m_ID = ROTATE;

	//family�� owner�� AnimateComponent�� �ʱ�ȭ��
}
CRotateComponent::~CRotateComponent() {

}
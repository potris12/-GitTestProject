#include "stdafx.h"
#include "FunctionPart.h"

//----------------------------dxobject-----------------------------
bool CFunctionPart::Begin() {
	//m_partID = part_id::PART_END;

	return CPart::Begin();
}
bool CFunctionPart::End() {

	return CPart::End();
}
//----------------------------dxobject-----------------------------

void CFunctionPart::Animate(float fTimeElapsed, XMMATRIX& xmmtxParents) {
	//Function parts�� �θ� parts�� �������� ������ ���� �� ���� ���� �θ� �ִ��� ���� �˻����� �ʴ´�.

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4Local), xmmtxParents));

}

void CFunctionPart::GetPartsList(list<CPart*>& lParts) {
	//functionpart�� ������ ��ǰ�̴ϱ� �ڱ� �ڽ��� �ִ´�.
	lParts.push_back(this);
}

bool CFunctionPart::SetParent(int slot, CPart* pParent) {
	if (nullptr == pParent) return false;

	m_pParent = pParent;

	return true;
}

bool CFunctionPart::SetLocalLocation(int slot, CPart* pParent) {

	switch (slot) {
	case 0:
		//right
		SetRight(-m_pParent->GetLook());
		SetUp(m_pParent->GetUp());
		SetLook(m_pParent->GetRight());
		SetPosition(m_pParent->GetPosition());
		break;
	case 1:
		//up
		SetRight(m_pParent->GetRight());
		SetUp(-m_pParent->GetLook());
		SetLook(m_pParent->GetUp());
		SetPosition(m_pParent->GetPosition());
		break;
	case 2:
		//look
		SetRight(m_pParent->GetRight());
		SetUp(m_pParent->GetUp());
		SetLook(m_pParent->GetLook());
		SetPosition(m_pParent->GetPosition());
		break;
	case 3:
		//-right
		SetRight((m_pParent->GetLook()));
		SetUp(-m_pParent->GetUp());
		SetLook(-m_pParent->GetRight());
		SetPosition(m_pParent->GetPosition());
		break;
	case 4:
		//-up
		SetRight(-m_pParent->GetRight());
		SetUp(m_pParent->GetLook());
		SetLook(-m_pParent->GetUp());
		SetPosition(m_pParent->GetPosition());
		break;
	case 5:
		//-look
		SetRight(-m_pParent->GetRight());
		SetUp(-m_pParent->GetUp());
		SetLook(-m_pParent->GetLook());
		SetPosition(m_pParent->GetPosition());
		break;
	default:
		return false;
		break;
	}

	//function part�� ����� �����Ǵ� vector�� look vector�̱� ������ ȸ�� �� �̵��� ���־�� �Ѵ�.
	//���� ���� look
	XMVECTOR xmvDir = XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.f);
	xmvDir = XMVector3Normalize(xmvDir);
	//�̵� ���� ����
	xmvDir = xmvDir*m_pParent->GetSize();

	XMFLOAT3 xmf3Dir;
	XMStoreFloat3(&xmf3Dir, xmvDir);
	m_xmf4x4Local._41 += xmf3Dir.x;
	m_xmf4x4Local._42 += xmf3Dir.y;
	m_xmf4x4Local._43 += xmf3Dir.z;

	return true;
}
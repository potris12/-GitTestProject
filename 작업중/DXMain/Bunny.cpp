#include "stdafx.h"
#include "Bunny.h"

bool CBunny::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_FBX_BUNNY;
	//m_objectID = object_id::OBJECT_TEST;

	CGameObject::Begin();
//	Rotate(-90.f, 0.f, 0.f);
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	return true;
}
bool CBunny::End() {

	return CGameObject::End();
}
void CBunny::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if (m_pTerrainContainer) m_xmf4x4World._42 = GetTerrainHeight();

}

CBunny::CBunny() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CBunny::~CBunny() {

}
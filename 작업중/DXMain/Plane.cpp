#include "stdafx.h"
#include "Plane.h"

bool CPlane::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_PLANE;
	CGameObject::Begin();
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	return true;
}
bool CPlane::End() {

	return CGameObject::End();
}


CPlane::CPlane() : CGameObject("plane", tag::TAG_STATIC_OBJECT) {

}
CPlane::~CPlane() {

}
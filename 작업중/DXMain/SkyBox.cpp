#include "stdafx.h"
#include "SkyBox.h"

bool CSkyBox::Begin() {
	//object_id set 
	m_objectID = object_id::OBJECT_SKYBOX;
	CGameObject::Begin();
	return true;
}
bool CSkyBox::End() {

	return CGameObject::End();
}
void CSkyBox::Animate(float fTimeElapsed) {
	//��� ������Ʈ�� ���鼭 Update����
	if (m_pCamera) SetPosition(m_pCamera->GetPosition());
	//SetPosition(XMVECTOR(XMLoadFloat3(&XMFLOAT3(0,0,0))));
}

void CSkyBox::SetCamera(shared_ptr<CCamera> pCamera){
	m_pCamera = pCamera;
}

CSkyBox::CSkyBox() : CGameObject("skybox", tag::TAG_STATIC_OBJECT) {

}
CSkyBox::~CSkyBox() {

}
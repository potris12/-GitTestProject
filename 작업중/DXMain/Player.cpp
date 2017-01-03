#include "stdafx.h"
#include "Player.h"
//----------------------------object-----------------------------
bool CPlayer::Begin() {
	//m_pControlObject = new CCore();
	m_pControlObject->Begin();
	CGameObject::Begin();
	return true;
}
bool CPlayer::End() {
	if (m_pControlObject) delete m_pControlObject;

	return true;
}
//----------------------------object-----------------------------

//----------------------------------------input------------------------
void CPlayer::ProcessInput(float fTimeElapsed, UCHAR* pKeyBuffer, float cxDelta, float cyDelta) {
	DWORD dwDirection = 0;
	//UINT iMoveState;


	if (pKeyBuffer[VK_W] & 0xF0)		dwDirection |= DIR_FORWARD;
	if (pKeyBuffer[VK_S] & 0xF0)		dwDirection |= DIR_BACKWARD;
	if (pKeyBuffer[VK_A] & 0xF0)		dwDirection |= DIR_LEFT;
	if (pKeyBuffer[VK_D] & 0xF0)		dwDirection |= DIR_RIGHT;
	if (pKeyBuffer[VK_E] & 0xF0)		dwDirection |= DIR_UP;
	if (pKeyBuffer[VK_Q] & 0xF0)		dwDirection |= DIR_DOWN;

	//if (pKeyBuffer[VK_SHIFT] & 0xF0)	iMoveState = RUN;
	//else								iMoveState = WALK;



#define SPEED 20

	XMVECTOR xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);

	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)		xmvShift += m_pControlObject->GetLook();
		if (dwDirection & DIR_BACKWARD)		xmvShift -= m_pControlObject->GetLook();
		if (dwDirection & DIR_RIGHT)		xmvShift += m_pControlObject->GetRight();
		if (dwDirection & DIR_LEFT)			xmvShift -= m_pControlObject->GetRight();
		if (dwDirection & DIR_UP)			xmvShift += m_pControlObject->GetUp();
		if (dwDirection & DIR_DOWN)			xmvShift -= m_pControlObject->GetUp();

		XMVector3Normalize(xmvShift);

		//pos�� shift*speed*ftimeElapsed ������ = �ӵ���ŭ �̵�
		m_pControlObject->Move(xmvShift, SPEED * fTimeElapsed);

	}

	if (cxDelta || cyDelta)
	{
		/*cxDelta�� y-���� ȸ���� ��Ÿ���� cyDelta�� x-���� ȸ���� ��Ÿ����. ������ ���콺 ��ư�� ������ ��� cxDelta�� z-���� ȸ���� ��Ÿ����.*/
		if (pKeyBuffer[VK_RBUTTON] & 0xF0)
			m_pControlObject->Rotate(cyDelta, 0.0f, -cxDelta);
		else
			m_pControlObject->Rotate(cyDelta, cxDelta, 0.0f);
	}

	m_pControlObject->Animate(fTimeElapsed);
}
//----------------------------------------input------------------------

	
CPlayer::CPlayer() : CGameObject("player", tag::TAG_DYNAMIC_OBJECT){

}
CPlayer::~CPlayer() {

}

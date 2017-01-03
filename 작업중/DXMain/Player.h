#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject{
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------
	
	//ȸ��
	//virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	//�̵�
	//virtual void Move(XMVECTOR xmvDir, float fDistance);

	//----------------------------------------input------------------------
	virtual void ProcessInput(float fTimeElapsed, UCHAR* pKeyBuffer, float cxDelta, float cyDelta);
	//----------------------------------------input------------------------

	CGameObject* GetControlObject() { return m_pControlObject; }
private:
	//�ڽ��� �����ϴ� ��ü
	CGameObject* m_pControlObject{ nullptr };

public:
	CPlayer();
	virtual ~CPlayer();

};
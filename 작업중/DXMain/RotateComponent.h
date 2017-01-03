#pragma once
#include "AnimateComponent.h"

class CRotateComponent : public CAnimateComponent {
public:
	//------------------------------���� ����� ��� �ϴ� �Լ�---------------------------
	virtual bool Begin();
	virtual bool End();
	virtual XMMATRIX Animate(XMMATRIX xmmtxWorld, float fTimeElapsed);
	//------------------------------���� ����� ��� �ϴ� �Լ�---------------------------
protected:
	float m_fRotate{ 0 };
	
public:

	CRotateComponent();
	virtual ~CRotateComponent();
};
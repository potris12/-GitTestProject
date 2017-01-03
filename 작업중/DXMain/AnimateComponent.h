#pragma once
#include "Component.h"
#include "GameObject.h"

class CAnimateComponent :public CComponent {
public:
	//------------------------------���� ����� ��� �ϴ� �Լ�---------------------------
	virtual bool Begin();
	virtual bool End();
	virtual XMMATRIX Animate(XMMATRIX xmmtxWorld, float fTimeElapsed) { return XMMatrixIdentity(); }
	//------------------------------���� ����� ��� �ϴ� �Լ�---------------------------

	//------------------------------������Ʈ Update----------------------------
	virtual void Update(float fTimeElapsed);
	//------------------------------������Ʈ Update----------------------------

	
protected:

public:

	CAnimateComponent();
	virtual ~CAnimateComponent();

};
#pragma once

#include "Part.h"
#include "Object.h"
#include "BoostPart.h"

//--test--
#include "RotateComponent.h"

class CCore : public CPart {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//-----------------------------part inform func-------------------------------
	//core�� state���� ���� - input, ai etc..
	//virtual void ChangeCoreState();
	//part�� state���� ���� - delete, new ..
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart);
	void NewPart(int slot, CPart* pPart);
	//-----------------------------part inform func-------------------------------

	//-------------------------object Animate---------------------------
	//CObject Animate���� - �� �Լ� �ȿ��� part���� �������� Animate
	virtual void Animate(float fTimeElapsed);
	//-------------------------object Animate---------------------------

	//------------------------equip func--------------------------------
	virtual bool SetEquip(int slot, CPart* pPart);
	//------------------------equip func--------------------------------

	//-----------------------------prepare Render------------------------
	void GetPartsList(list<CPart*>& lParts);
	//-----------------------------prepare Render------------------------

	virtual void RegistToContainer();
private:
	//--------------------------------velocity--------------------------
	//boost ��ǰ�� �޷��ִ� ��ġ�� ���� �޶���
	//rotarion velocity
	//rul -rul 0������ 5������ 
	float* m_pfRotationVelocity{ nullptr };
	//--------------------------------velocity--------------------------
	
	CPart** m_ppPart{ nullptr };//�ڽ��� ���� ������ part 6��

public:
	CCore();
	virtual ~CCore();
};

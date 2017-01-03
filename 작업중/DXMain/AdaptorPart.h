#pragma once

#include "Part.h"
#include "Object.h"
#include "BoostPart.h"

//--test--
#include "RotateComponent.h"

class CAdaptorPart : public CPart {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//-----------------------------part inform func-------------------------------
	//virtual void ChangeCoreState();
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart);
	//-----------------------------part inform func-------------------------------

	//--------------------------part Animate------------------------------
	virtual void Animate(float fTimeElapsed, XMMATRIX& xmmtxParents);
	//--------------------------part Animate------------------------------

	//-----------------------------prepare Render------------------------
	void GetPartsList(list<CPart*>& lParts);
	//-----------------------------prepare Render------------------------

	//------------------------equip func--------------------------------
	virtual bool SetEquip(int slot, CPart* pPart);
	//------------------------equip func--------------------------------
	
	//--------------------------equiped func--------------------------
	//parent
	virtual bool SetParent(int slot, CPart* pParent);
	//parent�� �������� offset�� �ξ� �ڽ��� ��ġ ����
	virtual bool SetLocalLocation(int slot, CPart* pParent);
	//--------------------------equiped func--------------------------

	virtual void RegistToContainer();

private:
	CPart** m_ppPart{ nullptr };//�ڽ��� ������ ��ǰ 6��
	int m_nParentSlot{ 0 };//�ڽ��� �θ� ��ǰ �� �ִ� ����

public:
	CAdaptorPart();
	virtual ~CAdaptorPart();
};

/*
core -> part���� /part = 5���� �ڽ��� ������ Ʈ�� ����
brunch = adaptorpart �ڽ��� �������� core���� �˸��� ���� (core�� ������ ����)
leaf = functionpart ����� ���� ����
*/
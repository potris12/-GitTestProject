#pragma once

#include "GameObject.h"

//enum part_id {
//	PART_CORE,
//	PART_ADAPTOR,
//	PART_WEAPON,
//	PART_BOOST,
//	PART_END
//};
enum part_state {
	STATE_NEW,
	STATE_DELETE,
	STATE_END
};

struct ABILITY_VALUE {
	float m_fsize{ 0 };
	float m_fVelocity{ 0 };
	ABILITY_VALUE(float size, float velocity) : m_fsize(size), m_fVelocity(velocity) {}
};

class CPart : public CGameObject {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------part inform func----------------------------
	//core�� state���� ���� - input, ai etc..
	virtual void ChangeCoreState() { };
	//part�� state���� ���� - delete, new ..etc 
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart) { };
	//--------------------------part inform func----------------------------

	//--------------------------part Animate------------------------------
	//part�鸸�� Ư���� �������� Animate 
	//�߽�Animate�� Core��.
	virtual void Animate(float fTimeElapsed, XMMATRIX& xmmtxParent) { };
	//--------------------------part Animate------------------------------

	//--------------------------prepare Render-----------------------
	//�ڽ��� ���� ��ǰ ���� list�� set core���� �����ؼ� ��� ��ǰ�� ���鼭 list�� ���� �ִ´�.
	virtual void GetPartsList(list<CPart*>& lParts) {};
	//--------------------------prepare Render-----------------------


	//---------------------------equip-------------------------------
	//�ڽ��� ���Կ� part���� �� ��� Equiped�Լ� ȣ��
	bool Equip(int slot, CPart* pAdaptor);
	//��� part�� parent part�ν� �����ϰ� parent��ġ�� ���� offset�� �־� �ڽ��� ��ġ�� ����
	bool Equiped(int slot, CPart* pAdaptor);
	//---------------------------equip-------------------------------


	//-------------------------set getter--------------------------------
	//part render container�� ���� id
	//part_id GetPartID() { return m_partID; }
	//offset������ ���� size���� 
	float GetSize() { return m_fSize; }
	virtual ABILITY_VALUE GetAbilityValue() { return ABILITY_VALUE(0,0); };

	//localmtx
	void SetLocalMtx(XMMATRIX mtxLocal) { XMStoreFloat4x4(&m_xmf4x4Local, mtxLocal); }
	XMMATRIX GetLocalMtx() { return XMLoadFloat4x4(&m_xmf4x4Local); }
	//------------------------equip func--------------------------------
	//�ش� ���Կ� part����!
	virtual bool SetEquip(int slot, CPart* pParent) { return false; };
	//------------------------equip func--------------------------------

	//------------------------equiped func--------------------------------
	//parent
	//parent�� �������� offset�� �ξ� �ڽ��� ��ġ ����
	virtual bool SetLocalLocation(int slot, CPart* pParent) { return false; };
	//parent�� set�ϰ� �ڽ��� ��ġ�� parent�� size�� offset���� �ξ� ��ġ����
	virtual bool SetParent(int slot, CPart* pParent) { return false; };
	//------------------------equiped func--------------------------------

	virtual CPart* GetParent() { return m_pParent; }
	//-------------------------set getter--------------------------------

	//virtual void RegistToContainer(mapRenderContainer& mRenderContainer);

protected:
	//�ڽ��� hp
	float m_fHP{ 0 };
	float m_fSize{ 0 };

	//part_id
	//part_id m_partID{ PART_END };

	//�ڽ��� �θ� ������
	CPart* m_pParent{ nullptr };

	//-------------------------------localmtx----------------------------
	XMFLOAT4X4 m_xmf4x4Local;
	//-------------------------------localmtx----------------------------
public:
	CPart() : CGameObject("part", tag::TAG_DYNAMIC_OBJECT) { };
	virtual ~CPart() { };
};
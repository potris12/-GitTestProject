#pragma once
class CGameObject;

enum component_id {
	ROTATE,
	COMPONENTID_END
};

enum family_id {
	ANIMATE,
	FAMILYID_END
};


class CComponent{
public:
	virtual bool Begin();
	virtual bool End();

	//�ش� ������Ʈ Update
	virtual void Update(float fTimeElapsed) = 0;

	//familyid, �� id get
	const family_id GetFamilyID() { return m_FamilyID; };
	const component_id GetID() { return m_ID; };

	//owner set, get
	void SetOwner(CGameObject* pObject);
	CGameObject* GetOwner() { return m_pOwner; }
protected:
	//������Ʈ id
	component_id m_ID;

	//���� ������ ������Ʈ�� ��Ƴ��� �йи� id
	//ex) geometry ����� render�� �׳� render
	family_id m_FamilyID;

	CGameObject* m_pOwner;

public:

	CComponent();
	virtual ~CComponent();
};


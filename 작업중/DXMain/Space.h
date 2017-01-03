#pragma once

#include "GameObject.h"
#include "Camera.h"

class CSpaceContainer;


class CSpace : public CGameObject {
public:
	void Begin(CSpaceContainer* m_pSpaceContainer, UINT size, int lv, XMVECTOR pos);
	virtual bool End();

	virtual void Animate(float fTimeElapsed);
	virtual void PrepareRender(shared_ptr<CCamera> pCamera);
	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);

	void SetbRender(bool b) { m_bRender = b; }
	bool GetbRender() { return m_bRender; }
	//virtual void SetGlobalBufferInfo(void** ppMappedResources);

	//�ش� space���� lay �浹 �˻�� ��ü �� ���� ���� ����� ��ü
	CGameObject* PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance);
private:
	//�ڽ� space 4��
	CSpace** m_ppChildSpace;
	UINT m_size{ 0 };//size
	int m_index{ -1 };//index -1�� �����̰ų�. brunch space���̴�.
	bool m_bRender{ false };//�ش� frame�� render�� ������ true��.
	//AABB aabb;

	//��ü��
	map<tag, list<CGameObject*>> m_mlpObject;
	//space container
	CSpaceContainer* m_pSpaceContainer{ nullptr };
public:
	CSpace();
	~CSpace();

};
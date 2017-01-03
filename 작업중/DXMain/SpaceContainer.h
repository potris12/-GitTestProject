#pragma once
#include "Object.h"
#include "Space.h"
#include "Camera.h"
//#include "RenderContainerSeller.h"


class CSpaceContainer : public CObject {
public:
	void Begin(int size, int lv);
	bool End();

	//space controller
	void Animate(float fTimeElapsed);
	void PrepareRender(shared_ptr<CCamera> pCamera);
	//animate�ϴٰ� �ش� ������ ��� ��ü �ӽ� �����
	void AddBlockObjectList(CGameObject* pObject);
	void AddObject(CGameObject* pObject);
	void ClearBlockObjectList();
	int SearchSpace(XMVECTOR xmvPos);
	void AddSpace(UINT index, CSpace* pSpace);
	//space controller

	//space
	list<CGameObject*>& GetBlockObjectList() { return m_lpBlockObject; }
	CSpace* GetStartSpace() { return m_pStartSpace; }
	UINT GetSpaceNum() { return m_nSpace; }
	UINT GetSize() { return m_size; }
	UINT GetLevel() { return m_level; }
	UINT GetOneSideSpaceNum() { return m_oneSideSpaceNum; }
	UINT GetOneSpaceSize() { return m_oneSpaceSize; }
	//�ڽ��� �ִ� ������ ã�� �Լ� �ش� ������ ��ȣ�� �����Ѵ�.
	

	//�ش� ������ �ε��� xyz�� spacenum���� �ٲٴ� �Լ�
	//int GetSpaceIndexNum(Index& index);
	//�ش� ������ �ε����� ��ȿ���� �˻��ϴ� �Լ�
	//bool CheckSpaceNum(Index& index);

	//��� lay �浹 �˻�� ��ü �� ���� ���� ����� ��ü
	CGameObject* PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse);
private:
	
	//space���� �����Ѵ�.
	CSpace** m_ppSpace{ nullptr };
	//���� Ʈ���� ��Ʈ��忡 �ش��ϴ� startSpace ����
	CSpace* m_pStartSpace{ nullptr };
	//block object list
	list<CGameObject*> m_lpBlockObject;

	//��ü ���� ũ��
	int m_size{ 0 };
	//������ ���� 
	int m_level{ 0 };
	//�� ���̵忡 �ִ� ������ ����
	int m_oneSideSpaceNum{ 0 };
	//���� �ϳ��� ũ��
	int m_oneSpaceSize{ 0 };
	//������ ����
	int m_nSpace{ 0 };

public:
	CSpaceContainer();
	~CSpaceContainer();
};
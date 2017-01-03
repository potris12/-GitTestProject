#include "stdafx.h"
#include "SpaceContainer.h"

void CSpaceContainer::Begin(int size, int lv){
	//ũ�� ���� 
	m_size = size;
	//���� ����
	m_level = lv;
	//�� ���̵忡 �ִ� ������ ����
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	//���� �ϳ��� ũ��
	m_oneSpaceSize = size / m_oneSideSpaceNum;
	//������ ���� ����Ʈ���ϱ� ����
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));

	//�����Ǿ��� leaf spaces
	m_ppSpace = new CSpace*[m_nSpace];
	
	//���� �Ҵ�.
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));

}

bool CSpaceContainer::End(){
	for (auto pObject : m_lpBlockObject) {
		pObject->End();
		delete pObject;
	}

	m_pStartSpace->End();

	return false;
}

void CSpaceContainer::Animate(float fTimeElapsed){
	
	//all space animate
	m_pStartSpace->Animate(fTimeElapsed);

	//Render �׸��׸��� space��� �� �ʱ�ȭ
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->SetbRender(false);
	}

	//animate ���Ŀ� �и��� block object�� �����Ѵ�.
	if (false == m_lpBlockObject.empty()) {//block object list�� empty�� �ƴ϶��
		for (auto pObject : m_lpBlockObject) {//��ü �ٽ� ��ġ
			AddObject(pObject);
		}
		m_lpBlockObject.clear();
	}
}

void CSpaceContainer::PrepareRender(shared_ptr<CCamera> pCamera){
	m_pStartSpace->PrepareRender(pCamera);
}

void CSpaceContainer::AddBlockObjectList(CGameObject * pObject){
	m_lpBlockObject.emplace_back(pObject);
}

void CSpaceContainer::AddObject(CGameObject * pObject){
	//���� index ���ϱ�
	int current_index = SearchSpace(pObject->GetPosition());


	if (current_index < 0) {
		current_index = 0;
		pObject->SetPosition(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	}

	//�ش� ������ ��ü �߰�
	m_ppSpace[current_index]->AddObject(pObject);
	//��ü�� ����index ���
	pObject->SetSpaceIndex(current_index);
}

void CSpaceContainer::ClearBlockObjectList() {
	m_lpBlockObject.clear();
}

int CSpaceContainer::SearchSpace(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	int x = static_cast<int>(xmf3Pos.x);
	//int y = xmf3Pos.y;
	int z = static_cast<int>(xmf3Pos.z);


	//�ڽ��� ��ġ�� �̿��� �ε����� ����
	int indexX = (int)x / m_oneSpaceSize;
	//int indexY = (int)y / m_oneSpaceSize;
	int indexZ = (int)z / m_oneSpaceSize;



	int index = indexX + (indexZ * m_oneSideSpaceNum);

	//���� ó�� ���� ��
	if (index < 0 || index >= m_nSpace) {
		return -1;
		//return 0;
	}
	if (x < 0 || z < 0 ||
		x > m_size || z > m_size) {
		return -1;
		//return 0;
	}
	//���� ó�� ���� ��


	return index;
}

void CSpaceContainer::AddSpace(UINT index, CSpace * pSpace){
	m_ppSpace[index] = pSpace;
}

CGameObject * CSpaceContainer::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse){
	float fHitDistance = FLT_MAX;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pHitObj = nullptr;
	CGameObject* pNearObj = nullptr;

	//�ڽ��� ��� leaf space�� ���ؼ� �˻�
	for (int i = 0; i < m_nSpace; ++i) {
		if (m_ppSpace[i]->GetbRender()) {//���� �׸��� �׸��� space���
			pHitObj = m_ppSpace[i]->PickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance);
			if (fNearHitDistance > fHitDistance) {
				fNearHitDistance = fHitDistance;
				pNearObj = pHitObj;
			}
		}
	}
	return pNearObj;
}

CSpaceContainer::CSpaceContainer() : CObject("spacecontainer"){
}

CSpaceContainer::~CSpaceContainer()
{
}


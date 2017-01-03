#include "stdafx.h"
#include "Space.h"
#include "SpaceContainer.h"
#include "RenderContainerSeller.h"

void CSpace::Begin(CSpaceContainer * pSpaceContainer, UINT size, int lv, XMVECTOR pos){
	//object_id set 
	m_objectID = object_id::OBJECT_SPACE;
	CGameObject::Begin();
	//size�� �˾ƾ� ������ �𸣴ϱ� �ϴ� ����
	m_size = size;
	m_pSpaceContainer = pSpaceContainer;

	//�ڽ��� ��ġ�� ������ 
	//������ �߰��� �ƴ϶� ���� ó��/ ��ǥ�� ���� ��ǥ
	SetPosition(pos);

	//���� ���� --
	lv -= 1;

	//0���� ������ return ������ �ܰ��� ��
	/*
	ex) ó�� lootSpace�� Begin�� lv�� 1�� �־��ٸ�
	���ͼ� -1 �ϰ���? �׷� 0 �̾� 0���� ���� �ʾ�
	�׷� �ڽ��� 4�� ���鲨��. 1�� 1�ܰ�
	0�� �������� ����.
	*/
	if (lv < 0) {
		//�ڽ��� ������ ���� -> leaf space��
		//leaf space�� �ڽ��� index�� �˰� �־�� ��
		//�ڽ��� index�� ��� �� ����
		m_index = m_pSpaceContainer->SearchSpace(GetPosition());
		//space container�� �ڽ��� ���
		m_pSpaceContainer->AddSpace(m_index, this);
		//SetRenderContainer(pSeller);//�׸��� �׸� ���� ������ RenderContainer set
		return;
	}

	//�ڽ� ���� �Ҵ�
	m_ppChildSpace = new CSpace*[4];
	//aabb
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));


	int k = 0;
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 2; ++i) {
			UINT child_size = (size / 2);
			//x���� ����, ���� z����.
			/* �ڽ� ������ ����
			2 3
			0 1 
			*/
			XMVECTOR xmvOffset = XMVectorSet(static_cast<float>(i*child_size), 0.f, static_cast<float>(j*child_size), 0.f);
			m_ppChildSpace[k] = new CSpace();
			m_ppChildSpace[k++]->Begin(pSpaceContainer, child_size, lv, GetPosition() + xmvOffset);
		}
	}

}

bool CSpace::End(){
	for (auto mlp : m_mlpObject) {
		for (auto pObject : mlp.second) {
			pObject->End();
			delete pObject;
		}
		mlp.second.clear();
	}
	m_mlpObject.clear();

	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->End();
		}
	}
	
	delete this;

	return true;
}

void CSpace::Animate(float fTimeElapsed){
	

	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->Animate(fTimeElapsed);
		}
	}
	else {//leaf space�� animate
		//dynamic object�� animate 
		//animate�ϴµ� ������ space index�� ���� index�� �ٸ��ٸ� 
		int nObject = 0;

		vector<CGameObject*> lpDeleteObject;
		for (auto pObject : m_mlpObject[tag::TAG_DYNAMIC_OBJECT]) {
			pObject->Animate(fTimeElapsed);
			int current_index = m_pSpaceContainer->SearchSpace(pObject->GetPosition());
			if (pObject->GetSpaceIndex() != current_index) {//���� ���� index�� ���� index�� �ٸ��ٸ�
				m_pSpaceContainer->AddBlockObjectList(pObject);//block Object list�� ���
				lpDeleteObject.push_back(pObject);
			}
			nObject++;
		}
		if(INPUTMGR->GetDebugMode())
			DEBUGER->AddText(10.0f, 10.0f, static_cast<float>(m_index * 15.f), YT_Color(255, 255, 255), L"space %d object_num : %d", m_index, nObject);


		for (auto pObject : lpDeleteObject) {
			RemoveObject(pObject);//�ش� ��ü remove
		}

	}
}

void CSpace::PrepareRender(shared_ptr<CCamera> pCamera){
	
	if (IsVisible(pCamera)) {//���⿡ space �������� �ø�
		if (nullptr == m_ppChildSpace) {//�� �ڽ��� ������ ���� leaf node
			//���� �׸��� space��.
			SetbRender(true);

			//RegistToContainer();
//			DEBUGER->RegistToDebugRenderContainer(this);
			for (auto mlp : m_mlpObject) {//��� ��ü�� ���ؼ�
				for (auto pObject : mlp.second) {
					pObject->RegistToContainer();//�ڽ��� ���� rendercontainer�� ���
				}
			}//end for
		}//end if
		else {//leaf�� �ƴ϶��
			for (int i = 0; i < 4; ++i) {

				m_ppChildSpace[i]->PrepareRender(pCamera);//�� �ڽĵ� PrePareRender
			}
		}//end else
	}
	//ī�޶� ������ ������ 
	return;
}


void CSpace::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		//��ü�� �ڽ� ��ȣ ���
		ppObjects[i]->SetSpaceIndex(m_index);
		m_mlpObject[ppObjects[i]->GetTag()].emplace_back(ppObjects[i]);
	}
}

void CSpace::AddObject(CGameObject* pObject) {
	if (!pObject) return;
	//��ü�� �ڽ� ��ȣ ���
	pObject->SetSpaceIndex(m_index);
	m_mlpObject[pObject->GetTag()].emplace_back(pObject);
}
void CSpace::RemoveObject(CGameObject* pObject) {
	if (!pObject) return;
	//if (0 == m_lpObjects.size()) return;

	m_mlpObject[pObject->GetTag()].remove_if([&pObject](CGameObject* pO) {
		return pObject == pO;
	});
}

CGameObject * CSpace::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance){
	float fHitDistance = FLT_MAX;
	distance = fHitDistance;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pObj = nullptr;
	//�ڽ��� ��� ��ü�� ���ؼ� �˻�
	for (auto pObject : m_mlpObject[tag::TAG_DYNAMIC_OBJECT]) {
		if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray�� �浹�ߴٸ�
			if (fNearHitDistance > fHitDistance) {//������ ���� ����� �༮�� ��
				distance = fHitDistance;//�� ������ ���� ����� ��ü ����
				pObj = pObject;
			}
		}
	}

	return pObj;//�ش� ��ü return
}


CSpace::CSpace() : CGameObject("space"){

}

CSpace::~CSpace()
{
}

#pragma once
#include "DXObject.h"
#include "GameObject.h"
#include "RenderContainerSeller.h"

class CAnimateContainer : public DXObject{
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin(CRenderContainerSeller* pSeller);
	virtual bool End();

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};
	//---------------------------dxobject---------------------------------

	//--------------------------animate container---------------------------------
	//animate �˻� �� �ش� �����̳ʿ��� ������� ��ü�� ExcludeContainer�� ���� �ִ´�.
	virtual void Animate(float fTimeElapsed, CAnimateContainer* pExcludeContainer);

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	list<CGameObject*>& GetObjectList() { return m_lpObjects; }
	void ClearObjectList() { m_lpObjects.clear(); }//�׸� �� �׸��� clear
	//--------------------------animate container---------------------------------

protected:

	//--------------------------���� ���------------------------------
	//�����̳��� ��ü���� �������� ���� ���̴�. �׷��Ƿ� ����Ʈ�� �����ϴ� ���� �´�
	list<CGameObject*> m_lpObjects;
	//--------------------------���� ���------------------------------

public:
	//animate container�� ī�޶� �������� �ø��� �������� �����Ƿ� camera�� �ʿ� ����.
	CAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CAnimateContainer();


};
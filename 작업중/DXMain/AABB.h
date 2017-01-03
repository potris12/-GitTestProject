#pragma once

#include "GameObject.h"


struct VS_VB_AABB_INSTANCE {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Extend;
};

class AABB : public CGameObject{
public:
	//debug buffer controll
	virtual void SetDebugBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	//buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	void SetBoundingBoxInfo(CGameObject* pObject);

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//���������� �༮���� �ٽ� ������� �ʿ䰡 ����
private:
	
public:
	AABB();
	~AABB();
};

#pragma once
#pragma once
#include "Object.h"

//class CRenderContainerSeller;
class CRenderContainer;
//�̳��� ���� ������ ������ �ֱ� ����..
//�ͷ����� ź����Ų ��ü�ν� ���� ��������� ������ set�ϱ� ���� ��ü�̴�.
//����, ��ü �������� ������ ������ �ش� ��ü�� ���� ������� �����͸� �������� �Ѵ�.
//�� ��ü�� ����ؼ� �ʿ��� ������ set�ϰ� setGlobalBufferinfo�Լ��� ����� �ȴ�.
//������ global buffer data�� �����ϴ°�ü
class CGlobalObject : public CObject {
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------globalobject--------------------------
	virtual void SetGlobalBufferInfo(void** ppMappedResources);
	virtual void SetDebugGlobalBufferInfo(void ** ppMappedResources);
	virtual void SetGlobalInfo(void* pData);
	//--------------------------globalobject--------------------------

	void UpdateGlobalBuffer();
	

protected:
	//object_id
	object_id m_objectID{ object_id::OBJECT_END };
	CRenderContainer* m_pRenderContainer{ nullptr };

public:
	CGlobalObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CGlobalObject();
};
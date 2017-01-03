#pragma once
#include "GameObject.h"

enum light_id {
	//LIGHT_AMBIENT, ambient light ������ DIRECTIONAL mesh�ȿ� ����ִ�.
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_CAPSULE,
	LIGHT_END
};

class CLight : public CGameObject {
public:
	bool Begin();
	virtual bool End() { return CGameObject::End(); };

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResource, int& nInstance, shared_ptr<CCamera> pCamera);
	
	//light info setter
	virtual void SetLength(float len) = 0;
	virtual void SetRange(float outer, float inner = 0.0f) = 0;
	virtual void SetColor(float r, float g, float b) = 0;//color

	light_id GetLightID() { return m_lightid; };


protected:
	//light�� ���۸� ���� ������ �ʴ´�.
	//rendercontainer�� ���� ����
	light_id m_lightid{ light_id::LIGHT_END };

public:
	CLight():CGameObject("light", tag::TAG_DYNAMIC_OBJECT) { m_lightid = light_id::LIGHT_END; }
	virtual ~CLight() {}
};

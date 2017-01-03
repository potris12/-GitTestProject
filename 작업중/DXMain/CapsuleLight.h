#pragma once

#include "Light.h"
#include "RotateComponent.h"

struct CAPSULE_LIGHT_DS_CB {
	XMMATRIX LightProjection;//world * view * projection
	
	float CapsuleLightRange;
	float HalfSegmentLen;
	XMFLOAT2 CapsuleTempData;//������ ������ float4 ���߷��� ���
};

struct CAPSULE_LIGHT_PS_CB {
	
	XMFLOAT3 CapsuleLightPos;//world._41 42 43
	float CapsuleLightRangeRcp;//range�� ������ ���

	XMFLOAT3 CapsuleLightDir;//look vector
	float CapsuleLightLen;

	XMFLOAT3 CapsuleLightColor;
	float CapsuleLightIntensity;
};

struct CAPSULE_LIGHT {
	float CapsuleLightLen;
	XMFLOAT3 CapsuleLightColor;
	float CapsuleLightRange;
	//float CapsuleLightIntensity;
};

class CCapsuleLight : public CLight {
public:
	virtual bool Begin(CAPSULE_LIGHT& light_info);
	virtual bool End();

	//instance buffer controll
	//mapped data�迭�� �޴´�. ������ �Լ� ������ �˾Ƽ�
	virtual void SetBufferInfo(void** ppMappedResource, int& nInstance, shared_ptr<CCamera> pCamera);

	//light info setter
	virtual void SetLength(float len);
	virtual void SetRange(float outer, float inner = 0.0f);
	virtual void SetColor(float r, float g, float b);//color
private:
	//data
	CAPSULE_LIGHT m_CapsuleData;
	//data
	float m_fHalfSegmentLen{ 0.0 };
	float m_fCapsuleLightRangeRcp{ 0.0 };

public:
	CCapsuleLight();
	virtual ~CCapsuleLight();
};
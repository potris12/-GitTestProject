#pragma once

#include "AnimateContainer.h"
#include "TestObject.h"
#include "Core.h"
#include "WeaponPart.h"
#include "BoostPart.h"
#include "AdaptorPart.h"
#include "Plane.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CapsuleLight.h"
#include "SpotLight.h"

#include "RenderContainerSeller.h"

class CTestAnimateContainer : public CAnimateContainer {
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

public:
	//animate container�� ī�޶� �������� �ø��� �������� �����Ƿ� camera�� �ʿ� ����.
	CTestAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CTestAnimateContainer();


};

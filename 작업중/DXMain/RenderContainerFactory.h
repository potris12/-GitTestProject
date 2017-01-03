#pragma once
#include "DXObject.h"
#include "RenderContainer.h"
#include "Object.h"
#include "Camera.h"


//�� ���丮�� product�� return�ϴ� �����̳ʴ�.
class CRenderContainerFactory : public DXObject {
public:
	//�̰Ÿ� �Ҹ�
	CRenderContainer* CreateRenderContainer(int nMaxObjects);
protected:
	shared_ptr<CCamera> m_pCamera{ nullptr };
	CRenderContainer* m_pContainer{ nullptr };

private:
	//����ִ� RenderContainer�� ������ ���� �Ҵ��ؼ� ���� ä�� ������.
	
	virtual void CreateContainer() {};
	virtual void CreateMesh() {};
	virtual void CreateSharedBuffer(int nMaxObjects) {};
	virtual void CreateShader() {};
	virtual void CreateTexture() {};
	virtual void CreateMaterial() {};
	virtual void BeginRenderContainer() {};

public:
	CRenderContainerFactory(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CRenderContainerFactory() {};
};

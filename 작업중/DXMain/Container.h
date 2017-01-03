#pragma once
#include "DXObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"

//object, mesh, shader�� �����ϴ� �༮
//3������ ������ �׸� �׸��� ����� �޶��� ������ �����
//object���� �ɹ� ������ ������ object�� ����ȭ�� �߿���
//mesh, shader�� ����������

//�����̳��� 
//�Ѱ��� �߿��� ���� mesh, shader, object�� �����Ѵٴ� ��
class CContainer : public DXObject{
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute() {};
	void Render();
	void Animate(float fTimeElapsed);

	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CShader> pShader);

	void SetObejcts(int n, CObject** ppObjects);
	void AddObject(CObject* pObject);
	void RemoveObject(CObject* pObject);
	list<CObject*>& GetObjectList() { return m_lpObjects; }

	ID3D11Buffer* CreateInstancingBuffer(int nObject, UINT nStrides, void* pData);
	//--------------------------container---------------------------------
protected:
	//--------------------------instance-------------------------------
	int m_nMaxObjects{ 0 };
	UINT m_nInstancingBufferStrides{ 0 };
	UINT m_nInstancingBufferOffsets{ 0 };
	ID3D11Buffer* m_pd3dInstancingBuffer{ nullptr };
	UINT m_nInstance{ 1 };
	//--------------------------instance-------------------------------

	//--------------------------���� ���------------------------------
	//�����̳��� ��ü���� �������� ���� ���̴�. �׷��Ƿ� ����Ʈ�� �����ϴ� ���� �´�
	list<CObject*> m_lpObjects;

	shared_ptr<CMesh> m_pMesh{ nullptr };
	shared_ptr<CShader> m_pShader{ nullptr };
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//--------------------------���� ���------------------------------

public:
	CContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CContainer();

};
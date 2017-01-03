#pragma once
#include "DXObject.h"
#include "GameObject.h"

//dxobject
#include "Mesh.h"
#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "GlobalBuffer.h"
#include "Material.h"
//dxobject

#include "Camera.h"



class CRenderContainer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void UpdateGlobalBuffer();
private:
	virtual void RenderExcute();
	void RenderExcuteWithOutObject();
public:
	void Render(shared_ptr<CCamera> pCamera);
	void RenderWithOutObject(shared_ptr<CCamera> pCamera);

	shared_ptr<CMesh> GetMesh(UINT index = 0) { return m_vpMesh[index]; }
	//set�� �ƴ϶����� add��.
	void AddMesh(shared_ptr<CMesh> pMesh);
	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CRenderShader> pShader);
	
	//void AddMesh(shared_ptr<CMesh> pMesh);
	//void AddShader(shared_ptr<CRenderShader> pShader);
	void AddTexture(shared_ptr<CTexture> pTexture);
	void AddBuffer(shared_ptr<CBuffer> pBuffers);
	void AddInstanceBuffer(shared_ptr<CBuffer> pBuffers);
	void AddMaterial(shared_ptr<CMaterial> pMaterial);
	//global buffer
	void AddGlobalBuffer(shared_ptr<CGlobalBuffer> pGlobalBuffer);
	void SetGlobalObject(CGlobalObject* pGlobalObject) { m_pGlobalObject = pGlobalObject; }
	//global buffer

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.clear(); }//�׸� �� �׸��� clear
	list<CGameObject*>& GetObjectList() { return m_lpObjects; }

	void ClearTextures() { m_vpTexture.clear(); }
	//--------------------------container---------------------------------
protected:
	//--------------------------���� ���------------------------------
	//�����̳��� ��ü���� �������� ���� ���̴�. �׷��Ƿ� ����Ʈ�� �����ϴ� ���� �´�
	int m_nInstance{ 0 };
	list<CGameObject*> m_lpObjects;

	vector<shared_ptr<CMesh>> m_vpMesh;
	shared_ptr<CRenderShader> m_pShader;
	vector<shared_ptr<CTexture>> m_vpTexture;
	int m_nTexture{ 0 };
	vector<shared_ptr<CBuffer>> m_vpBuffer;
	
	int m_nBuffer{ 0 };
	//global buffer
	vector<shared_ptr<CGlobalBuffer>> m_vpGlobalBuffer;
	int m_nGlobalBuffer{ 0 };
	CGlobalObject* m_pGlobalObject{ nullptr };
	//global buffer
	vector<shared_ptr<CMaterial>> m_vpMaterial;
	int m_nMaterial{ 0 };
	//--------------------------���� ���------------------------------

	void** m_ppBufferData{ nullptr };
	void** m_ppGlobalBufferData{ nullptr };
public:
	CRenderContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CRenderContainer();

};

typedef map<object_id, CRenderContainer*> mapRenderContainer;
typedef pair<object_id, CRenderContainer*> pairRenderContainer;


/*
�����̳� - AnimateContainer, RenderContainer�� �и�
{
���� ���� - 
	1. AnimateContainer�� ��� object Animate
	2. �׸� �׸� object RenderContainer�� AddObject
	3. RenderContainer���� ī�޶� �������� �ø� �� �ν��Ͻ����� ����
���⿡ �������� ����?
CScene�� Space�� �����Ѵ�.
Spcae�� Container�� �����ϴµ� AnimateContainer��.
Animate�� �� �߿��Ѱ� ������ ��� ��� �ٷ� �ٸ� ������ �̽��ϴ� ���� �ƴ϶�
���� �����Ҵٰ� �־� ��� �Ѵ�. pExcludeContainer
���⼭ ExcluderContainer�� ��ü���� ���� ������ �� �� ������

----------------------------------------------------
AnimateContainer/ object_id{enum} �ʿ伺 ���

enum���� 0-nObject ��ü �������� enum�ؼ� �̰� objectid ������ ������ �־
������ �з��� �� �� �ִٸ� AnimateContainer�� object�� �߻�ȭ�� �� ���ִٴ� �����Ͽ�
��ü���� ��� �ǰ� �ϳ��� �������� �ϳ����� ������ �ȴ�.
----------------------------------------------------

----------------------------------------------------
RenderContainer/ object���� �ʿ��ϴ�
RenderContainer�� instancing�ؾ� �ϴϱ� ���� �� ��ü���� �����ؾ� �Ѵ�.
���� RenderContainer�� CScene�� �����ϰ�
Scene�� RenderContainer�� map���� �����Ѵ�. map<object_id, RenderContainer> 

AnimateContainer���� enum�̷� �з��� ������ �ؼ� RenderContainer���� �Ѱ��ָ�
�װ� �޾Ƽ� ó���ϵ��� �Ѵ�.

RenderContainer�� CScene�� �����ϴ°�
}
*/
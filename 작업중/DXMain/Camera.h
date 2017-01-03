#pragma once
#include "DXObject.h"



struct GBUFFER_UNPACKING_DATA {
	XMFLOAT4 PerspectiveValues;
	XMFLOAT4X4 ViewInv;
};

struct stCameraBuffer {
	XMFLOAT4X4 m_xmf4x4ViewProjection;
	//directional
	XMFLOAT4 m_CameraPos;
};

class CPlayer;

class CCamera :public DXObject{
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState() {	};
	virtual void CleanShaderState() { }

	void UpdateShaderState();
	//----------------------------dxobject-----------------------------


	//flustum culling
	bool IsInFrustum(BoundingBox& boundingBox);
	//---------------------------------------ī�޶� ����---------------------------------
	//lookat
	void SetLookAt(XMVECTOR& xmvPos, XMVECTOR& xmvLookAt, XMVECTOR& xmvUp);
	//position ���� ����
	virtual void SetPosition(XMVECTOR pos);
	//ȸ��
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;
	//Ani ī�޶� �׼� = CObject�� Animate - Component���
	

	//----------------------------------------���� �Ҹ� �Լ���------------------------
	//player����ٴϱ�
	virtual void Update(float fTimeElapsed) = 0;
	virtual void ProcessInput(float fTimeElapsed) = 0;
	//----------------------------------------���� �Ҹ� �Լ���------------------------

	//debug���� ī�޶� �׸��� 
	virtual void Render() {};
	//world mtx ���� �� ��ȯ render�� �� ��� 
	virtual XMMATRIX GetWorldMtx();
	//---------------------------------------ī�޶� ����---------------------------------


	

	//---------------------------------------ī�޶� ���� ------------------------------
	//projectionMtx����
	void GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar);
	//viewmtx ����
	void UpdateViewMtx();
	//���۴� �ƴ����� viewport
	void SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ);
	//---------------------------------------ī�޶� ���� ------------------------------

	//player
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	//player

	ID3D11Buffer* GetViewProjectionBuffer() { return m_pViewProjectionBuffer; }
	ID3D11Buffer* GetGBufferUnpackingBuffer() { return m_pGBufferUnpackingBuffer; }
	XMMATRIX GetViewMtx() { return XMLoadFloat4x4(&m_xmf4x4View); }
	XMMATRIX GetProjectionMtx() { return XMLoadFloat4x4(&m_xmf4x4Projection); }

	D3D11_VIEWPORT& GetViewport() { return m_d3dViewport; }

	RECT GetClientSize() { return m_rcClient; };
	XMVECTOR GetPosition() { return XMLoadFloat3(&m_xmf3Pos); }
protected:
	//aabb flustum
	BoundingFrustum m_OriBoundingFrustum;
	BoundingFrustum m_BoundingFrustum;

	//rect size
	RECT m_rcClient;
	//player
	CPlayer* m_pPlayer{ nullptr };

	//ī�޶� �������
	stCameraBuffer m_stCameraBufferData;
	ID3D11Buffer* m_pViewProjectionBuffer;
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;

	//viewport
	D3D11_VIEWPORT m_d3dViewport;

	//ī�޶� 4���� CObject�� worldmtx�� ������ �����.
	XMFLOAT3 m_xmf3Right;
	XMFLOAT3 m_xmf3Up;
	XMFLOAT3 m_xmf3Look;
	XMFLOAT3 m_xmf3Pos;

	//-----------------------------------deferred light test--------------------
	GBUFFER_UNPACKING_DATA m_GBufferUnpackingData;
	ID3D11Buffer* m_pGBufferUnpackingBuffer{ nullptr };
public:
	CCamera(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CCamera();

};
#include "stdafx.h"
#include "TerrainMesh.h"


bool CTerrainMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

	//������ü �޽��� 2���� ���� ����(��ġ ���� ���ۿ� ���� ����)�� �����ȴ�.
	//������ü �޽��� ���� ����(��ġ ���� ����)�� �����Ѵ�.
	m_pVertices = new XMFLOAT3[4];
	int i = 0;
	/*
		0 0 -> 0, 1
		x 0 -> 1, 1
		0 z -> 0, 0
		x z -> 1, 0
	*/
	m_pVertices[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pVertices[i++] = XMFLOAT3(ONESPACE_SIZE, 0.0f, 0.0f);
	m_pVertices[i++] = XMFLOAT3(0.0f, 0.0f, ONESPACE_SIZE);
	m_pVertices[i++] = XMFLOAT3(ONESPACE_SIZE, 0.0f, ONESPACE_SIZE);

	//calc TBN
	XMFLOAT3 T[4];
	XMFLOAT3 B[4];
	XMFLOAT2 UV[4] = { {0,1},{ 1,1 },{ 0,0 },{ 1,0 } };
	CreateTBFromPoints(m_pVertices, UV, T[0], B[0]);
	//calc TBN

	for (int j = 1; j < 4; ++j) {
		T[j] = T[0];
		B[j] = B[0];
	}
	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, T, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, B, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//-----------------------------------���� ����-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer,m_pd3dTBuffer,m_pd3dBBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3),sizeof(XMFLOAT3) ,sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[3] = { 0,0,0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------���� ����-----------------------------------

//	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
//	
//	//-----------------------------------���� ����-----------------------------------
//	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dPositionBuffer };
//	UINT pnBufferStrides[1] = { sizeof(XMFLOAT3) };
//	UINT pnBufferOffsets[1] = { 0 };
//	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
//	//-----------------------------------���� ����-----------------------------------

	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(ONESPACE_SIZE, ONESPACE_SIZE, ONESPACE_SIZE, 0.f));


	if (m_ppd3dVertexBuffers) return true;
	//derectional light mesh�� ���� 0���� hlsl�ڵ忡�� �˾Ƽ� �����Ѵ�.

	return false;
}
bool CTerrainMesh::CreateIndexBuffer() {

	return true;
}
CTerrainMesh::CTerrainMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CTerrainMesh::~CTerrainMesh() {

}
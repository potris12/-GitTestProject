#include "stdafx.h"
#include "TestDeferredMesh.h"

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 10
#define HEIGHT 10
#define DEPTH 10

bool CTestDeferredMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	//ȭ�� ��ǥ�� 4�� �ش�
	VS_VB_SCREEN v[4];
	v[0].xmf3Position = XMFLOAT4(-1, -1, 0.5, 1);
	v[1].xmf3Position = XMFLOAT4(-1, 1, 0.5, 1);
	v[2].xmf3Position = XMFLOAT4(1, -1, 0.5, 1);
	v[3].xmf3Position = XMFLOAT4(1, 1, 0.5, 1);

	v[0].xmf3TextCoord = XMFLOAT2(0, 1);
	v[1].xmf3TextCoord = XMFLOAT2(0, 0);
	v[2].xmf3TextCoord = XMFLOAT2(1, 1);
	v[3].xmf3TextCoord = XMFLOAT2(1, 0);


	m_pVertices = new XMFLOAT3[m_nVertices];
	m_pVertices[0] = XMFLOAT3(-1.0f, -1.0f, 0.01f);
	m_pVertices[1] = XMFLOAT3(-1.0f, 1.0f, 0.01f);
	m_pVertices[2] = XMFLOAT3(1.0f, -1.0f, 0.01f);
	m_pVertices[3] = XMFLOAT3(1.0f, 1.0f, 0.01f);

	XMFLOAT2 pxmf2TexCoords[4];

	//������ü�� �� ��(�ﰢ�� 2��)�� �ϳ��� �ؽ��� �̹��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	pxmf2TexCoords[0] = XMFLOAT2(0, 1);
	pxmf2TexCoords[1] = XMFLOAT2(0, 0);
	pxmf2TexCoords[2] = XMFLOAT2(1, 1);
	pxmf2TexCoords[3] = XMFLOAT2(1, 0);


	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	//������ ��ġ ����, ���� ����, �ؽ��� ��ǥ�� ���´�.
	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CTestDeferredMesh::CreateIndexBuffer() {

	return true;

}
CTestDeferredMesh::CTestDeferredMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CTestDeferredMesh ::~CTestDeferredMesh() {

}
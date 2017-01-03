#include "stdafx.h"
#include "TestMesh.h"

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 10
#define HEIGHT 10
#define DEPTH 10

bool CTestMesh::CreateVertexBuffer() {
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 pxmf2TexCoords[36];
	int i = 0;

	//������ü�� �� ��(�ﰢ�� 2��)�� �ϳ��� �ؽ��� �̹��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	m_pVertices[i] = XMFLOAT3(-fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(-fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(-fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(+fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, +fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pVertices[i] = XMFLOAT3(+fx, +fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, +fz);
	pxmf2TexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pVertices[i] = XMFLOAT3(+fx, -fy, -fz);
	pxmf2TexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//���� ���͸� �����ϱ� ���� ���� �ڵ带 �߰��Ѵ�.
	XMVECTOR pxmvNormals[36];
	CalculateVertexNormal(pxmvNormals);
	XMFLOAT3 pxmf3Normals[36];
	for (int j = 0; j < 36; ++j) {
		XMStoreFloat3(&pxmf3Normals[j], pxmvNormals[j]);
	}
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Normals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
	//������ ��ġ ����, ���� ����, �ؽ��� ��ǥ�� ���´�.
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CTestMesh::CreateIndexBuffer() {

	return true;


}
CTestMesh::CTestMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CTestMesh::~CTestMesh() {

}
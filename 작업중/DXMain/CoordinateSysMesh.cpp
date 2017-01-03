#include "stdafx.h"
#include "CoordinateSysMesh.h"

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 10
#define HEIGHT 10
#define DEPTH 10

bool CCoordinateSysMesh::CreateVertexBuffer() {
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	float fx = WIDTH, fy = HEIGHT, fz = DEPTH;

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 pxmf2TexCoords[6];
	int i = 0;

	//������ü�� �� ��(�ﰢ�� 2��)�� �ϳ��� �ؽ��� �̹��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(+fx, 0.f, 0.f);

	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(0.f, +fy, 0.f);

	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, 0.f);
	m_pVertices[i++] = XMFLOAT3(0.f, 0.f, +fz);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//������ ��ġ ����, ���� ����, �ؽ��� ��ǥ�� ���´�.
	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dPositionBuffer };
	UINT pnBufferStrides[1] = { sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(0.0f, 0.0f, 0.0f, 0.f), XMVectorSet(fx, fy, fz, 0.f));

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CCoordinateSysMesh::CreateIndexBuffer() {

	return true;


}
CCoordinateSysMesh::CCoordinateSysMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CCoordinateSysMesh::~CCoordinateSysMesh() {

}